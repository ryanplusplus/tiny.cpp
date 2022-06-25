/*!
 * @file
 * @brief Runs one-shot and periodic timers using client-allocated memory.
 */

#ifndef tiny_TimerGroup_hpp
#define tiny_TimerGroup_hpp

#include <cstdint>
#include <cstdbool>
#include "tiny/ITimeSource.hpp"
#include "tiny/List.hpp"

namespace tiny {
  typedef uint32_t TimerTicks;

  class TimerGroup;

  class Timer : public List::Node {
    friend class TimerGroup;

   public:
    Timer() = default;
    Timer(const Timer& other) = delete;

    auto operator=(const Timer& other) -> void = delete;

   private:
    typedef void (*Callback)(void* context);

   private:
    void* context{};
    Callback callback{};
    TimerTicks start_ticks{};
    TimerTicks expiration_ticks{};
    bool periodic{};
  };

  class TimerGroup {
   public:
    explicit TimerGroup(ITimeSource& time_source);
    TimerGroup(const TimerGroup& other) = delete;

    auto operator=(const TimerGroup& other) -> void = delete;

    auto run() -> TimerTicks;

    auto stop(const Timer& timer) -> void
    {
      timers.remove(timer);
    }

    auto is_running(const Timer& timer) -> bool
    {
      return timers.contains(timer);
    }

    auto remaining_ticks(const Timer& timer) -> TimerTicks;

    template <typename Context>
    auto start(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context)) -> void
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), false);
    }

    auto start(Timer& timer, TimerTicks ticks, void (*callback)(void*)) -> void
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), false);
    }

    template <typename Context>
    auto start_periodic(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context)) -> void
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), true);
    }

    auto start_periodic(Timer& timer, TimerTicks ticks, void (*callback)(void*)) -> void
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), true);
    }

   private:
    auto _start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic) -> void;
    auto add_timer(Timer& timer) -> void;
    auto pending_ticks() -> ITimeSource::TickCount;

   private:
    ITimeSource& time_source;
    List timers{};
    ITimeSource::TickCount last_time_source_ticks;
    TimerTicks current_ticks{};
  };
}

#endif
