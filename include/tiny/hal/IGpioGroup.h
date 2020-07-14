/*!
 * @file
 * @brief
 */

#ifndef IGpioGroup_h
#define IGpioGroup_h

#include <cstdint>
#include "tiny/hal/IGpio.h"

namespace tiny
{
  class IGpioGroup
  {
   public:
    typedef uint8_t Channel;

    enum class Direction : uint8_t
    {
      Input,
      Output
    };

   public:
    auto virtual set_direction(Channel channel, Direction direction) -> void = 0;
    auto virtual read(Channel channel) -> bool = 0;
    auto virtual Write(Channel channel, bool state) -> void = 0;
  };
}

#endif
