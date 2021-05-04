/*!
 * @file
 * @brief Runs one-shot timers using client-allocated memory.
 */

#ifndef TimerGroup_h
#define TimerGroup_h

#include <cstdint>
#include <cstdbool>
#include "tiny/ITimeSource.h"
#include "tiny/List.h"

namespace tiny {
  typedef uint32_t TimerTicks;

  class TimerGroup;

  class Timer {
    friend class TimerGroup;

   public:
    typedef void (*Callback)(void* context, TimerGroup& group);

   public:
    Timer();

   private:
    List::Node node;
    void* context;
    Callback callback;
    TimerTicks start_ticks;
    TimerTicks remaining_ticks;
    bool periodic;
  };

  class TimerGroup {
   public:
    TimerGroup(ITimeSource& time_source);

    auto run() -> TimerTicks;
    auto stop(Timer& timer) -> void;
    auto is_running(Timer& timer) -> bool;
    auto remaining_ticks(Timer& timer) -> TimerTicks;

    template <typename Context>
    auto start(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context, TimerGroup& group)) -> void
    {
      this->_start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), false);
    }

    template <typename Context>
    auto start_periodic(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context, TimerGroup& group)) -> void
    {
      this->_start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), true);
    }

   private:
    auto _start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic) -> void;
    auto add_timer(Timer& timer) -> void;

   private:
    ITimeSource& time_source;
    List timers;
    ITimeSource::TickCount last_ticks;
    TimerTicks next_ready;
  };
}

#endif
