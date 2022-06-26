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
    virtual void set_direction(Direction direction) = 0;
    virtual bool read() = 0;
    virtual void write(bool state) = 0;
  };
}

#endif
