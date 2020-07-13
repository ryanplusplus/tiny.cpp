/*!
 * @file
 * @brief
 */

#include "tiny_Crc16.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(tiny_Crc16){};

TEST(tiny_Crc16, should_seem_to_work)
{
  CHECK_EQUAL(0x1021, Crc16::calculate(0x0000, 0x01));
}
