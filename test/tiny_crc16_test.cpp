/*!
 * @file
 * @brief
 */

#include "tiny_crc16.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(tiny_crc16){};

TEST(tiny_crc16, should_seem_to_work)
{
  CHECK_EQUAL(0x1021, Crc16::calculate(0x0000, 0x01));
}
