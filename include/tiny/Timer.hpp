/*!
 * @file
 * @brief Runs one-shot and periodic timers using client-allocated memory.
 *
 * The tick resolution is not specified, but will generally be 1 millisecond.
 *
 * Timer durations may be longer than specified but will not be shorter (within
 * the limits of the tick resolution).
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
    bool expired{};
  };

  class TimerGroup {
   public:
    explicit TimerGroup(ITimeSource& time_source);
    TimerGroup(const TimerGroup& other) = delete;

    void operator=(const TimerGroup& other) = delete;

    /*!
     * Runs a timer group. Services at most one timer per call. Returns the number
     * of ticks until the next timer will be ready to run. This will generally be
     * called in the main loop.
     */
    TimerTicks run();

    /*!
     * Returns the ticks until the next timer will be ready to execute.
     */
    TimerTicks ticks_until_next_ready();

    /*!
     * Starts a timer.
     */
    template <typename Context>
    void start(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context))
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), false);
    }

    /*!
     * Starts a timer with NULL context..
     */
    void start(Timer& timer, TimerTicks ticks, void (*callback)(void*))
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), false);
    }

    /*!
     * Starts a periodic timer.
     */
    template <typename Context>
    void start_periodic(Timer& timer, TimerTicks ticks, Context* context, void (*callback)(Context* context))
    {
      _start(timer, ticks, context, reinterpret_cast<Timer::Callback>(callback), true);
    }

    /*!
     * Starts a periodic timer with NULL context.
     */
    void start_periodic(Timer& timer, TimerTicks ticks, void (*callback)(void*))
    {
      _start(timer, ticks, nullptr, reinterpret_cast<Timer::Callback>(callback), true);
    }

    /*!
     * Stops a timer.
     */
    void stop(const Timer& timer)
    {
      timers.remove(timer);
    }

    /*!
     * Returns true if the specified timer is running and false otherwise.
     */
    bool is_running(const Timer& timer) const
    {
      return timers.contains(timer);
    }

    /*!
     * Returns the remaining ticks for a running timer.
     */
    TimerTicks remaining_ticks(const Timer& timer) const;

   private:
    void _start(Timer& timer, TimerTicks ticks, void* context, Timer::Callback callback, bool periodic);
    void add_timer(Timer& timer);
    ITimeSource::TickCount pending_ticks() const;

   private:
    ITimeSource& time_source;
    List timers{};
    ITimeSource::TickCount last_time_source_ticks;
    TimerTicks current_ticks{};
  };
}

#endif
