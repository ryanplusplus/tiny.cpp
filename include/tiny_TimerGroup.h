/*!
 * @file
 * @brief Runs one-shot timers using client-allocated memory.
 */

#ifndef tiny_TimerGroup_h
#define tiny_TimerGroup_h

#include <cstdint>
#include "tiny_ITimeSource.h"
#include "tiny_List.h"

#include <iostream>

namespace tiny
{
  typedef uint16_t TimerTicks;

  class TimerGroup;

  class Timer
  {
    friend class TimerGroup;

   public:
    typedef void (*Callback)(void* context, TimerGroup* group);

    Timer()
      : node(), context(nullptr), callback(nullptr), remaining_ticks()
    {
    }

   private:
    List::Node node;
    void* context;
    Callback callback;
    TimerTicks remaining_ticks;
  };

  class TimerGroup
  {
   public:
    TimerGroup(ITimeSource* time_source)
      : time_source(time_source), timers(), last_ticks(time_source->ticks())
    {
    }

    auto run() -> bool
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

    template <typename Context>
    auto start(Timer* timer, TimerTicks ticks, Context* context, void (*callback)(Context* context, TimerGroup* group)) -> void
    {
      timer->context = reinterpret_cast<void*>(context);
      timer->callback = reinterpret_cast<Timer::Callback>(callback);
      timer->remaining_ticks = ticks;

      this->timers.remove(reinterpret_cast<List::Node*>(timer));
      this->timers.push_back(reinterpret_cast<List::Node*>(timer));
    }

    auto stop(Timer* timer) -> void
    {
      this->timers.remove(reinterpret_cast<List::Node*>(timer));
    }

    auto is_running(Timer* timer) -> bool
    {
      return this->timers.contains(reinterpret_cast<List::Node*>(timer));
    }

    auto remaining_ticks(Timer* timer) -> TimerTicks
    {
      return timer->remaining_ticks;
    }

   private:
    ITimeSource* time_source;
    List timers;
    ITimeSource::TickCount last_ticks;
  };
}

#endif
