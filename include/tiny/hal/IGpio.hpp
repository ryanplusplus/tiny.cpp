/*!
 * @file
 * @brief Abstract GPIO.
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

    /*!
     * Set the direction of the GPIO.
     */
    virtual void set_direction(Direction direction) = 0;

    /*!
     * Read the state of the GPIO (must be configured as an input).
     */
    virtual bool read() const = 0;

    /*!
     * Write the state of the GPIO (must be configured as an input).
     */
    virtual void write(bool state) = 0;
  };
}

#endif
