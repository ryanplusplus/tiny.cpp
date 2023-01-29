/*!
 * @file
 * @brief Time source test double.
 */

#ifndef tiny_test_double_TimeSource_hpp
#define tiny_test_double_TimeSource_hpp

#include "tiny/ITimeSource.hpp"

namespace tiny::test_double {
  class TimeSource final : public ITimeSource {
   public:
    TimeSource() = default;

    TimeSource(const TimeSource&) = delete;

    void operator=(const TimeSource& other) = delete;

    ITimeSource::TickCount ticks() const override
    {
      return _ticks;
    }

    /*!
     * Sets the current ticks.
     */
    void set_ticks(ITimeSource::TickCount ticks)
    {
      _ticks = ticks;
    }

    /*!
     * Moves the current ticks forward by the specified number of ticks.
     */
    void tick(ITimeSource::TickCount ticks = 1)
    {
      _ticks += ticks;
    }

   private:
    ITimeSource::TickCount _ticks{};
  };
}

#endif
