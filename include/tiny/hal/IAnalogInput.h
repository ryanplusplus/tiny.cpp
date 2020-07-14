/*!
 * @file
 * @brief
 */

#ifndef IAnalogInput_h
#define IAnalogInput_h

#include <cstdint>

namespace
{
  class IAnalogInput
  {
   public:
    typedef uint16_t Counts;

   public:
    auto virtual read() -> Counts = 0;
  };
}

#endif
