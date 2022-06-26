/*!
 * @file
 * @brief Abstraction for performing an asynchronous SPI transfer.
 *
 * Assumes chip select is managed by the client.
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
    void transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size,
      Context* context,
      void (*callback)(Context* context))
    {
      transfer(
        write_buffer,
        read_buffer,
        buffer_size,
        context,
        reinterpret_cast<IAsyncSpi::Callback>(callback));
    }

    /*!
     * Performs a simultaneous write/read. If not reading or writing, the corresponding
     * buffer can be left NULL.
     *
     * Clients should assume that the callback is raised from an interrupt.
     */
   private:
    virtual void transfer(
      const uint8_t* write_buffer,
      uint8_t* read_buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) = 0;
  };
}

#endif
