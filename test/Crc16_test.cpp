/*!
 * @file
 * @brief
 */

#include "tiny/Crc16.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(Crc16){};

TEST(Crc16, byte_should_seem_to_work)
{
  CHECK_EQUAL(0x1021, Crc16::byte(0x0000, 0x01));
}

TEST(Crc16, block_should_seem_to_work)
{
  uint8_t block[] = {1, 2, 3};
  CHECK_EQUAL(0x6131, Crc16::block(0x0000, block, sizeof(block)));
}
