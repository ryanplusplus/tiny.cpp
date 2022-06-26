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

    void operator=(const Timer& other) = delete;

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

    void operator=(const TimerGroup& other) = delete;

    TimerTicks run();

    void stop(const Timer& timer)
    {
      timers.remove(timer);
    }

    bool is_running(const Timer& timer)
    {
      return timers.contains(timer);
    }

    TimerTicks remaining_ticks(const Timer& timer);

    template <typename Context>
    void start(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context))
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), false);
    }

    void start(Timer& timer, TimerTicks ticks, void (*callback)(void*))
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), false);
    }

    template <typename Context>
    void start_periodic(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context))
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), true);
    }

    void start_periodic(Timer& timer, TimerTicks ticks, void (*callback)(void*))
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), true);
    }

   private:
    void _start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic);
    void add_timer(Timer& timer);
    ITimeSource::TickCount pending_ticks();

   private:
    ITimeSource& time_source;
    List timers{};
    ITimeSource::TickCount last_time_source_ticks;
    TimerTicks current_ticks{};
  };
}

#endif
