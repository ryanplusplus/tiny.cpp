/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef IAsyncSpi_h
#define IAsyncSpi_h

#include <cstdint>

namespace tiny
{
  class IAsyncSpi
  {
   public:
    typedef void (*Callback)(void* context);

   public:
    auto virtual transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size,
      Callback callback,
      void* context) -> void = 0;
  };
}

#endif
