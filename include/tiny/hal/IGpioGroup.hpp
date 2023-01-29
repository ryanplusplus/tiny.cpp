/*!
 * @file
 * @brief Abstract group of GPIOs.
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

    /*!
     * Set the direction of the GPIO channel.
     */
    virtual void set_direction(Channel channel, Direction direction) = 0;

    /*!
     * Read the state of the GPIO channel (must be configured as an input).
     */
    virtual bool read(Channel channel) const = 0;

    /*!
     * Write the state of the GPIO channel (must be configured as an input).
     */
    virtual void write(Channel channel, bool state) = 0;
  };
}

#endif
