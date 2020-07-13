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

   public:
    Timer();

   private:
    List::Node node;
    void* context;
    Callback callback;
    TimerTicks remaining_ticks;
  };

  class TimerGroup
  {
   public:
    TimerGroup(ITimeSource* time_source);

    auto run() -> bool;
    auto stop(Timer* timer) -> void;
    auto is_running(Timer* timer) -> bool;
    auto remaining_ticks(Timer* timer) -> TimerTicks;

    template <typename Context>
    auto start(Timer* timer, TimerTicks ticks, Context* context, void (*callback)(Context* context, TimerGroup* group)) -> void
    {
      this->_start(timer, ticks, reinterpret_cast<void*>(context), reinterpret_cast<Timer::Callback>(callback));
    }

   private:
    auto _start(Timer* timer, TimerTicks ticks, void* context, Timer::Callback callback) -> void;

   private:
    ITimeSource* time_source;
    List timers;
    ITimeSource::TickCount last_ticks;
  };
}

#endif
