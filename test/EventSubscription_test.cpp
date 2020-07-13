/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny/EventSubscription.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(EventSubscription)
{
  uint8_t context;
  EventSubscription<uint16_t> subscription{&context, callback};

  static void callback(uint8_t * context, const uint16_t* args)
  {
    mock()
      .actualCall("callback")
      .withParameter("context", context)
      .withParameter("args", args);
  }
};

TEST(EventSubscription, should_invoke_callback_when_published)
{
  mock()
    .expectOneCall("callback")
    .withParameter("context", &context)
    .withParameter("args", (const uint16_t*)0x1234);

  subscription.publish((const uint16_t*)0x1234);
}
