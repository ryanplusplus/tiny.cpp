/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IGpio_hpp
#define tiny_hal_IGpio_hpp

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
    virtual auto set_direction(Direction direction) -> void = 0;
    virtual auto read() -> bool = 0;
    virtual auto write(bool state) -> void = 0;
  };
}

#endif
