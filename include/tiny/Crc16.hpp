/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_Crc16_hpp
#define tiny_Crc16_hpp

#include <cstdint>
#include <cstddef>

namespace tiny {
  class Crc16 {
   public:
    static uint16_t byte(uint16_t seed, uint8_t byte);
    static uint16_t block(uint16_t seed, const uint8_t* bytes, size_t byte_count);
  };
}

#endif
