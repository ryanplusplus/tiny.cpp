/*!
 * @file
 * @brief Timer group test double.
 */

#ifndef tiny_test_double_TimerGroup_hpp
#define tiny_test_double_TimerGroup_hpp

#include <algorithm>
#include "tiny/Timer.hpp"
#include "tiny/test_double/TimeSource.hpp"

namespace tiny::test_double {
  class TimerGroup {
   public:
    TimerGroup() = default;

    tiny::TimerGroup& test_double()
    {
      return _timer_group;
    }

    /*!
     * Elapses time and runs the timer group.
     */
    void elapse_time(tiny::TimerTicks ticks)
    {
      while(ticks) {
        auto ticks_to_elapse = std::min(ticks, _timer_group.ticks_until_next_ready());

        time_source.tick(ticks_to_elapse);
        _timer_group.run();

        ticks -= ticks_to_elapse;
      }
    }

   private:
    tiny::test_double::TimeSource time_source{};
    tiny::TimerGroup _timer_group{ time_source };
  };
}

#endif
