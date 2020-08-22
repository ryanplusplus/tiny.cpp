/*!
 * @file
 * @brief
 */

#ifndef time_source_double_h
#define time_source_double_h

#include "tiny/ITimeSource.h"

namespace tiny {
  class TimeSourceDouble : public ITimeSource {
   public:
    TimeSourceDouble()
      : _ticks()
    {
    }

    auto ticks() -> ITimeSource::TickCount
    {
      return this->_ticks;
    }

    auto set_ticks(ITimeSource::TickCount ticks) -> void
    {
      this->_ticks = ticks;
    }

    auto tick(ITimeSource::TickCount ticks = 1) -> void
    {
      this->_ticks += ticks;
    }

   private:
    ITimeSource::TickCount _ticks;
  };
}

#endif
