/*!
 * @file
 * @brief
 */

#include "tiny/Crc16.h"

using namespace tiny;

auto Crc16::byte(uint16_t seed, uint8_t byte) -> uint16_t
{
  auto crc = seed;
  byte = crc >> 8 ^ byte;
  byte ^= byte >> 4;
  return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
}

auto Crc16::block(uint16_t seed, const uint8_t* bytes, size_t byte_count) -> uint16_t
{
  auto crc = seed;
  for(auto i = static_cast<size_t>(0); i < byte_count; i++) {
    crc = byte(crc, bytes[i]);
  }
  return crc;
}
