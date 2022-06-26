/*!
 * @file
 * @brief
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

    template <typename Context>
    void write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      Context* context,
      Callback callback)
    {
      write(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        context,
        reinterpret_cast<Callback>(callback));
    }

    virtual void write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) = 0;

    template <typename Context>
    void read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      Context* context,
      Callback callback)
    {
      read(
        address,
        prepare_for_restart,
        buffer,
        buffer_size,
        context,
        reinterpret_cast<Callback>(callback));
    }

    virtual void read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      void* context,
      Callback callback) = 0;

    virtual void reset() = 0;
  };
}

#endif
