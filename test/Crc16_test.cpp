/*!
 * @file
 * @brief
 */

#include "tiny/Crc16.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(Crc16){};

TEST(Crc16, should_seem_to_work)
{
  CHECK_EQUAL(0x1021, Crc16::calculate(0x0000, 0x01));
}
