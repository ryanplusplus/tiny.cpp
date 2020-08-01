/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef ISpi_h
#define ISpi_h

#include <cstdint>

namespace tiny
{
  class ISpi
  {
   public:
    virtual ~ISpi(){};

    auto virtual transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size) -> void = 0;
  };
}

#endif
