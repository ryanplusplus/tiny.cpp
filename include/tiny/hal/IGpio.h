/*!
 * @file
 * @brief
 */

#ifndef IGpio_h
#define IGpio_h

#include <cstdint>

namespace tiny {
  class IGpio {
   public:
    enum class Direction : uint8_t {
      Input,
      Output
    };

   public:
    virtual ~IGpio(){};
    auto virtual set_direction(Direction direction) -> void = 0;
    auto virtual read() -> bool = 0;
    auto virtual write(bool state) -> void = 0;
  };
}

#endif
