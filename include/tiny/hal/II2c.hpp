/*!
 * @file
 * @brief Abstract synchronous I2C peripheral.
 */

#ifndef tiny_hal_II2c_hpp
#define tiny_hal_II2c_hpp

#include <cstdint>

namespace tiny {
  class II2c {
   public:
    virtual ~II2c(){};

    /*!
     * Writes bytes from a buffer to the specified address. The stop
     * condition can be omitted in order to allow for a repeated start
     * by setting prepare_for_restart.
     *
     * Returns true if the transaction succeeded and false otherwise.
     */
    virtual bool write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size) = 0;

    /*!
     * Read bytes into a buffer from the specified address. The stop
     * condition can be omitted in order to allow for a repeated start
     * by setting prepare_for_restart.
     *
     * Returns true if the transaction succeeded and false otherwise.
     */
    virtual bool read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size) = 0;

    /*!
     * Resets the bus.
     */
    virtual void reset() = 0;
  };
}

#endif
