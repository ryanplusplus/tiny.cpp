/*!
 * @file
 * @brief
 */

#ifndef tiny_test_double_TimeSource_h
#define tiny_test_double_TimeSource_h

#include "tiny/ITimeSource.h"

namespace tiny::test_double {
  class TimeSource final : public ITimeSource {
   public:
    TimeSource()
      : _ticks{}
    {
    }

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
