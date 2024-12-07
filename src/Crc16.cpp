/*!
 * @file
 * @brief
 */

#include "tiny/Crc16.hpp"

using namespace tiny;

uint16_t Crc16::byte(uint16_t seed, uint8_t byte)
{
  auto crc = seed;
  byte = static_cast<uint8_t>(crc >> 8 ^ byte);
  byte ^= static_cast<uint8_t>(byte >> 4);
  return static_cast<uint16_t>((crc << 8) ^ (static_cast<uint16_t>(byte << 12)) ^ (static_cast<uint16_t>(byte << 5)) ^ (static_cast<uint16_t>(byte)));
}

uint16_t Crc16::block(uint16_t seed, const uint8_t* bytes, size_t byte_count)
{
  auto crc = seed;
  for(auto i = static_cast<size_t>(0); i < byte_count; i++) {
    crc = byte(crc, bytes[i]);
  }
  return crc;
}
