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
    TimeSource()
      : _ticks{}
    {
    }

    TimeSource(const TimeSource&) = delete;

    auto ticks() -> ITimeSource::TickCount override
    {
      return _ticks;
    }

    auto set_ticks(ITimeSource::TickCount ticks) -> void
    {
      _ticks = ticks;
    }

    auto tick(ITimeSource::TickCount ticks = 1) -> void
    {
      _ticks += ticks;
    }

   private:
    ITimeSource::TickCount _ticks;
  };
}

#endif
