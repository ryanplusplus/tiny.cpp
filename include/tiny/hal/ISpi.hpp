/*!
 * @file
 * @brief Abstraction for performing a synchronous SPI transfer.
 *
 * Assumes chip select is managed by the client.
 */

#ifndef tiny_hal_ISpi_hpp
#define tiny_hal_ISpi_hpp

#include <cstdint>

namespace tiny {
  class ISpi {
   public:
    virtual ~ISpi(){};

    /*!
     * Perform a simultaneous write/read. If not reading or writing, the corresponding
     * buffer can be left NULL.
     */
    virtual void transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size) = 0;
  };
}

#endif
