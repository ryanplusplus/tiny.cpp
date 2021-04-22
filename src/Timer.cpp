/*!
 * @file
 * @brief
 */

#include <limits>
#include "tiny/Timer.h"

using namespace tiny;

Timer::Timer()
  : node(), context(nullptr), callback(nullptr), start_ticks(), remaining_ticks(), periodic()
{
}

TimerGroup::TimerGroup(ITimeSource& time_source)
  : time_source(time_source), timers(), last_ticks(time_source.ticks()), next_ready()
{
}

auto TimerGroup::run() -> TimerTicks
{
  auto current_ticks = this->time_source.ticks();
  auto delta = current_ticks - this->last_ticks;
  this->last_ticks = current_ticks;

  auto timer_ready = false;
  this->next_ready = std::numeric_limits<TimerTicks>::max();

  for(auto i = this->timers.begin(); i != this->timers.end(); ++i) {
    auto timer = reinterpret_cast<Timer*>(*i);

    if(delta < timer->remaining_ticks) {
      timer->remaining_ticks -= delta;

      if(timer->remaining_ticks < this->next_ready) {
        this->next_ready = timer->remaining_ticks;
      }
    }
    else {
      timer->remaining_ticks = 0;

      if(timer_ready) {
        this->next_ready = 0;
      }

      timer_ready = true;
    }
  }

  if(timer_ready) {
    for(auto i = this->timers.begin(); i != this->timers.end(); ++i) {
      auto timer = reinterpret_cast<Timer*>(*i);

      if(timer->remaining_ticks == 0) {
        if(!timer->periodic) {
          this->timers.remove(*i);
        }

        timer->callback(timer->context, *this);

        if(timer->periodic && this->is_running(*timer)) {
          timer->remaining_ticks = timer->start_ticks;
          this->add_timer(*timer);
        }

        break;
      }
    }
  }

  return this->next_ready;
}

auto TimerGroup::_start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic) -> void
{
  timer.context = context;
  timer.callback = callback;
  timer.start_ticks = ticks;
  timer.remaining_ticks = ticks;
  timer.periodic = periodic;

  this->add_timer(timer);
}

auto TimerGroup::add_timer(Timer& timer) -> void
{
  this->timers.remove(reinterpret_cast<List::Node*>(&timer));
  this->timers.push_back(reinterpret_cast<List::Node*>(&timer));

  if(timer.remaining_ticks < this->next_ready) {
    this->next_ready = timer.remaining_ticks;
  }
}
