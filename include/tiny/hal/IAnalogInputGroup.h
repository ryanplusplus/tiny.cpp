/*!
 * @file
 * @brief
 */

#ifndef IAnalogInputGroup_h
#define IAnalogInputGroup_h

#include <cstdint>

namespace tiny
{
  class IAnalogOutput
  {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    auto virtual read(Channel channel) -> Counts = 0;
  };
}

#endif
