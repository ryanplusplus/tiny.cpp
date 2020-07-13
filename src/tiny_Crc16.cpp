/*!
 * @file
 * @brief
 */

#include "tiny_Crc16.h"

using namespace tiny;

auto Crc16::calculate(uint16_t seed, uint8_t byte) -> uint16_t
{
  uint16_t crc = seed;
  byte = crc >> 8 ^ byte;
  byte ^= byte >> 4;
  return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
}
