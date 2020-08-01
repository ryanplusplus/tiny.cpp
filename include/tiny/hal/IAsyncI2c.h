/*!
 * @file
 * @brief
 */

#ifndef IAsyncI2c_h
#define IAsyncI2c_h

#include <cstdint>

namespace tiny
{
  class IAsyncI2c
  {
   public:
    typedef void (*Callback)(void* context, bool success);

   public:
    virtual ~IAsyncI2c(){};

    auto virtual write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size,
      Callback callback,
      void* context) -> void = 0;

    auto virtual read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size,
      Callback callback,
      void* context) -> void = 0;

    auto virtual reset() -> void = 0;
  };
}

#endif
