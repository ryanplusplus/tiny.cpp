/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_Crc16_h
#define tiny_Crc16_h

#include <cstdint>
#include <cstddef>

namespace tiny {
  class Crc16 {
   public:
    static auto byte(uint16_t seed, uint8_t byte) -> uint16_t;
    static auto block(uint16_t seed, const uint8_t* bytes, size_t byte_count) -> uint16_t;
  };
}

#endif
