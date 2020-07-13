/*!
 * @file
 * @brief Abstraction for getting the current system time. This can be implemented
 * using something like Arduino's millis() or by using a system tick interrupt to
 * keep time.
 */

#ifndef i_tiny_time_source_h
#define i_tiny_time_source_h

#include <cstdint>

namespace tiny
{
  class I_TimeSource
  {
   public:
    typedef uint16_t TickCount;

   public:
    auto virtual ticks() -> TickCount = 0;
  };
}

#endif
