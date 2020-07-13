/*!
 * @file
 * @brief Implements CRC16-CCITT (polynomial 0x1021).
 */

#ifndef tiny_Crc16_h
#define tiny_Crc16_h

#include <cstdint>

namespace tiny
{
  class Crc16
  {
   public:
    auto static calculate(uint16_t seed, uint8_t byte) -> uint16_t;
  };
}

#endif
