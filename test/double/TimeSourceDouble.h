/*!
 * @file
 * @brief
 */

#ifndef TimeSourceDouble_h
#define TimeSourceDouble_h

#include "tiny/ITimeSource.h"

namespace tiny {
  class TimeSourceDouble : public ITimeSource {
   public:
    TimeSourceDouble()
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
