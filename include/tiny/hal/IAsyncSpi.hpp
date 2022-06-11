/*!
 * @file
 * @brief Assumes chip select is managed by the client.
 */

#ifndef tiny_hal_IAsyncSpi_hpp
#define tiny_hal_IAsyncSpi_hpp

#include <cstdint>

namespace tiny {
  class IAsyncSpi {
   public:
    typedef void (*Callback)(void* context);

   public:
    virtual ~IAsyncSpi(){};

    template <typename Context>
    auto transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size,
      Context* context,
      void (*callback)(Context* context)) -> void
    {
      transfer(
        write_buffer,
        read_buffer,
        buffer_size,
        context,
        reinterpret_cast<IAsyncSpi::Callback>(callback));
    }

   private:
    virtual auto transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) -> void = 0;
  };
}

#endif
