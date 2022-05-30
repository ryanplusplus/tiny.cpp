/*!
 * @file
 * @brief
 */

#include <limits>
#include "tiny/Timer.h"

using namespace tiny;

Timer::Timer()
  : context{nullptr}, callback{nullptr}, start_ticks{}, remaining_ticks{}, periodic{}
{
}

TimerGroup::TimerGroup(ITimeSource& time_source)
  : time_source{time_source}, timers{}, last_ticks{time_source.ticks()}, next_ready{}
{
}

auto TimerGroup::run() -> TimerTicks
{
  auto current_ticks = time_source.ticks();
  auto delta = static_cast<ITimeSource::TickCount>(current_ticks - last_ticks);
  last_ticks = current_ticks;

  auto timer_ready = false;
  next_ready = std::numeric_limits<TimerTicks>::max();

  for(auto _timer : timers) {
    auto timer = reinterpret_cast<Timer*>(_timer);
    if(delta < timer->remaining_ticks) {
      timer->remaining_ticks -= delta;

      if(timer->remaining_ticks < next_ready) {
        next_ready = timer->remaining_ticks;
      }
    }
    else {
      timer->remaining_ticks = 0;

      if(timer_ready) {
        next_ready = 0;
      }

      timer_ready = true;
    }
  }

  if(timer_ready) {
    for(auto _timer : timers) {
      auto timer = reinterpret_cast<Timer*>(_timer);

      if(timer->remaining_ticks == 0) {
        if(!timer->periodic) {
          timers.remove(_timer);
        }

        timer->callback(timer->context);

        if(timer->periodic && is_running(*timer)) {
          timer->remaining_ticks = timer->start_ticks;
          add_timer(*timer);
        }

        break;
      }
    }
  }

  return next_ready;
}

auto TimerGroup::_start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic) -> void
{
  timer.context = context;
  timer.callback = callback;
  timer.start_ticks = ticks;
  timer.remaining_ticks = ticks;
  timer.periodic = periodic;

  add_timer(timer);
}

auto TimerGroup::add_timer(Timer& timer) -> void
{
  timers.remove(&timer);
  timers.push_back(&timer);

  if(timer.remaining_ticks < next_ready) {
    next_ready = timer.remaining_ticks;
  }
}
