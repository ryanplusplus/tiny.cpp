/*!
 * @file
 * @brief Runs one-shot and periodic timers using client-allocated memory.
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

  class Timer : public List::Node {
    friend class TimerGroup;

   public:
    Timer();

   private:
    typedef void (*Callback)(void* context);

   private:
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

    auto stop(Timer& timer) -> void
    {
      timers.remove(reinterpret_cast<List::Node*>(&timer));
    }

    auto is_running(Timer& timer) -> bool
    {
      return timers.contains(reinterpret_cast<List::Node*>(&timer));
    }

    auto remaining_ticks(Timer& timer) -> TimerTicks
    {
      return timer.remaining_ticks;
    }

    template <typename Context>
    auto start(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context)) -> void
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), false);
    }

    template <typename Context>
    auto start_periodic(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context)) -> void
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), true);
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
