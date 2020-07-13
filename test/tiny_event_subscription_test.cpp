/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny_event_subscription.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(tiny_event_subscription)
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

TEST(tiny_event_subscription, should_invoke_callback_when_published)
{
  mock()
    .expectOneCall("callback")
    .withParameter("context", &context)
    .withParameter("args", (const uint16_t*)0x1234);

  subscription.publish((const uint16_t*)0x1234);
}
