/*!
 * @file
 * @brief
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

    ITimeSource::TickCount ticks() override
    {
      return _ticks;
    }

    void set_ticks(ITimeSource::TickCount ticks)
    {
      _ticks = ticks;
    }

    void tick(ITimeSource::TickCount ticks = 1)
    {
      _ticks += ticks;
    }

   private:
    ITimeSource::TickCount _ticks{};
  };
}

#endif
