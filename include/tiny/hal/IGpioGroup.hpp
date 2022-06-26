/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IGpioGroup_hpp
#define tiny_hal_IGpioGroup_hpp

#include <cstdint>
#include "tiny/hal/IGpio.hpp"

namespace tiny {
  class IGpioGroup {
   public:
    typedef uint8_t Channel;

    enum class Direction : uint8_t {
      Input,
      Output
    };

   public:
    virtual ~IGpioGroup(){};
    virtual void set_direction(Channel channel, Direction direction) = 0;
    virtual bool read(Channel channel) = 0;
    virtual void Write(Channel channel, bool state) = 0;
  };
}

#endif
