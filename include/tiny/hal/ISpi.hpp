/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef tiny_hal_ISpi_hpp
#define tiny_hal_ISpi_hpp

#include <cstdint>

namespace tiny {
  class ISpi {
   public:
    virtual ~ISpi(){};

    virtual void transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size) = 0;
  };
}

#endif