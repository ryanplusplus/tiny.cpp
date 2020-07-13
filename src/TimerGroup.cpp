/*!
 * @file
 * @brief
 */

#include "tiny/TimerGroup.h"

using namespace tiny;

Timer::Timer()
  : node(), context(nullptr), callback(nullptr), remaining_ticks()
{
}

TimerGroup::TimerGroup(ITimeSource* time_source)
  : time_source(time_source), timers(), last_ticks(time_source->ticks())
{
}

auto TimerGroup::run() -> bool
{
  auto current_ticks = this->time_source->ticks();
  auto delta = current_ticks - this->last_ticks;
  this->last_ticks = current_ticks;

  auto invoked_callback = false;

  for(auto i = this->timers.begin(); i != this->timers.end(); ++i)
  {
    auto timer = reinterpret_cast<Timer*>(*i);

    if(delta < timer->remaining_ticks)
    {
      timer->remaining_ticks -= delta;
    }
    else
    {
      timer->remaining_ticks = 0;

      if(!invoked_callback)
      {
        invoked_callback = true;
        this->timers.remove(*i);
        timer->callback(timer->context, this);
      }
    }
  }

  return invoked_callback;
}

auto TimerGroup::stop(Timer* timer) -> void
{
  this->timers.remove(reinterpret_cast<List::Node*>(timer));
}

auto TimerGroup::is_running(Timer* timer) -> bool
{
  return this->timers.contains(reinterpret_cast<List::Node*>(timer));
}

auto TimerGroup::remaining_ticks(Timer* timer) -> TimerTicks
{
  return timer->remaining_ticks;
}

auto TimerGroup::_start(Timer* timer, TimerTicks ticks, void* context, Timer::Callback callback) -> void
{
  timer->context = context;
  timer->callback = callback;
  timer->remaining_ticks = ticks;

  this->timers.remove(reinterpret_cast<List::Node*>(timer));
  this->timers.push_back(reinterpret_cast<List::Node*>(timer));
}
