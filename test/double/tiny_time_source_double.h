/*!
 * @file
 * @brief
 */

#ifndef tiny_time_source_double_h
#define tiny_time_source_double_h

#include "i_tiny_time_source.h"

namespace tiny
{
  class TimeSourceTestDouble : public I_TimeSource
  {
   public:
    TimeSourceTestDouble()
      : _ticks()
    {
    }

    auto ticks() -> I_TimeSource::TickCount
    {
      return this->_ticks;
    }

    auto set_ticks(I_TimeSource::TickCount ticks) -> void
    {
      this->_ticks = ticks;
    }

    auto tick(I_TimeSource::TickCount ticks = 1) -> void
    {
      this->_ticks += ticks;
    }

   private:
    I_TimeSource::TickCount _ticks;
  };
}

#endif
