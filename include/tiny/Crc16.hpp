/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_Crc16_hpp
#define tiny_Crc16_hpp

#include <cstddef>
#include <cstdint>

namespace tiny {
  class Crc16 {
   public:
    /*!
     * Calculate CRC given seed and next byte.
     */
    static uint16_t byte(uint16_t seed, uint8_t byte);

    /*!
     * Calculate the CRC of a block given the seed.
     */
    static uint16_t block(uint16_t seed, const uint8_t* bytes, size_t byte_count);
  };
}

#endif
