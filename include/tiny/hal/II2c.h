/*!
 * @file
 * @brief
 */

#ifndef II2c_h
#define II2c_h

#include <cstdint>

namespace tiny
{
  class II2c
  {
   public:
    auto virtual write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size) -> bool = 0;

    auto virtual read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size) -> bool = 0;

    auto virtual reset() -> void = 0;
  };
}

#endif
