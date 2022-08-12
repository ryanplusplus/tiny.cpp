/*!
 * @file
 * @brief
 */

#include <limits>
#include "tiny/Timer.hpp"

using namespace tiny;

TimerGroup::TimerGroup(ITimeSource& time_source)
  : time_source{ time_source }, last_time_source_ticks{ time_source.ticks() }
{
}

TimerTicks TimerGroup::run()
{
  auto current_ticks = time_source.ticks();
  auto delta = static_cast<ITimeSource::TickCount>(current_ticks - last_time_source_ticks);
  last_time_source_ticks = current_ticks;

  auto last_ticks = this->current_ticks;
  this->current_ticks += delta;

  for(auto& _timer : timers) {
    auto& timer = reinterpret_cast<Timer&>(_timer);

    if(timer.expired) {
      continue;
    }

    TimerTicks remaining_ticks = timer.expiration_ticks - last_ticks;

    if(remaining_ticks <= delta) {
      timer.expired = true;
    }
    else {
      break;
    }
  }

  for(auto& _timer : timers) {
    auto& timer = reinterpret_cast<Timer&>(_timer);
    if(timer.expired) {
      if(!timer.periodic) {
        timers.remove(_timer);
      }

      timer.callback(timer.context);

      if(timer.periodic && is_running(timer)) {
        timer.expiration_ticks = current_ticks + timer.start_ticks;
        add_timer(timer);
      }
    }

    break;
  }

  for(auto& _timer : timers) {
    auto& timer = reinterpret_cast<Timer&>(_timer);
    return remaining_ticks(timer);
  }

  return std::numeric_limits<TimerTicks>::max();
}

TimerTicks TimerGroup::remaining_ticks(const Timer& timer)
{
  TimerTicks remaining = timer.expiration_ticks - current_ticks;
  ITimeSource::TickCount pending = pending_ticks();

  if(!timer.expired && (remaining > pending)) {
    return remaining - pending;
  }
  else {
    return 0;
  }
}

void TimerGroup::_start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic)
{
  timer.periodic = periodic;
  timer.context = context;
  timer.callback = callback;
  timer.start_ticks = ticks;
  timer.expiration_ticks = current_ticks + ticks + pending_ticks();

  add_timer(timer);
}

void TimerGroup::add_timer(Timer& timer)
{
  timers.remove(timer);

  timer.expired = false;

  Timer* after = nullptr;
  auto _remaining_ticks = remaining_ticks(timer);

  for(auto& _timer : timers) {
    auto& timer = reinterpret_cast<Timer&>(_timer);
    if(_remaining_ticks >= remaining_ticks(timer)) {
      after = &timer;
    }
    else {
      break;
    }
  }

  if(after) {
    timers.insert_after(*after, timer);
  }
  else {
    timers.push_front(timer);
  }
}

ITimeSource::TickCount TimerGroup::pending_ticks()
{
  return time_source.ticks() - last_time_source_ticks;
}
