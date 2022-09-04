/*!
 * @file
 * @brief Abstract asynchronous I2C peripheral.
 */

#ifndef tiny_hal_IAsyncI2c_hpp
#define tiny_hal_IAsyncI2c_hpp

#include <cstdint>

namespace tiny {
  class IAsyncI2c {
   public:
    typedef void (*Callback)(void* context, bool success);

   public:
    virtual ~IAsyncI2c(){};

    /*!
     * Writes bytes from a buffer to the specified address. The stop
     * condition can be omitted in order to allow for a repeated start
     * by setting prepare_for_restart.
     *
     * Clients should assume that the callback could be invoked from
     * the interrupt context.
     */
    template <typename Context>
    void write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      Context* context,
      void (*callback)(Context* context))
    {
      _write(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        context,
        reinterpret_cast<Callback>(callback));
    }

    void write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      Callback callback)
    {
      _write(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        nullptr,
        callback);
    }

    /*!
     * Read bytes into a buffer from the specified address. The stop
     * condition can be omitted in order to allow for a repeated start
     * by setting prepare_for_restart.
     *
     * Clients should assume that the callback could be invoked from
     * the interrupt context.
     */
    template <typename Context>
    void read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      Context* context,
      void (*callback)(Context* context))
    {
      _read(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        context,
        reinterpret_cast<Callback>(callback));
    }

    void read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      Callback callback)
    {
      _read(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        nullptr,
        callback);
    }

    /*!
     * Reset the I2C bus.
     */
    virtual void reset() = 0;

   private:
    virtual void _write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) = 0;

    virtual void _read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) = 0;
  };
}

#endif
