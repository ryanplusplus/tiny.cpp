/*!
 * @file
 * @brief
 */

#ifndef IGpio_h
#define IGpio_h

#include <cstdint>

namespace tiny
{
  class IGpio
  {
   public:
    enum class Direction : uint8_t
    {
      Input,
      Output
    };

   public:
    auto virtual set_direction(Direction direction) -> void;
    auto virtual read() -> bool;
    auto virtual write(bool state) -> void;
  };
}

#endif
