/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny_single_subscriber_event.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(tiny_single_subscriber_event)
{
  uint16_t context;
  SingleSubscriberEvent<uint8_t> instance;
  EventSubscription<uint8_t> subscription{&context, subscriber};

  void setup()
  {
  }

  static void subscriber(uint16_t * context, const uint8_t* args)
  {
    mock()
      .actualCall("subscriber")
      .withParameter("context", context)
      .withParameter("args", args);
  }
};

TEST(tiny_single_subscriber_event, should_do_nothing_when_published_with_no_subscribers)
{
  instance.publish((const uint8_t*)0x1234);
}

TEST(tiny_single_subscriber_event, should_publish_to_all_subscribers)
{
  instance.subscribe(&subscription);

  mock()
    .expectOneCall("subscriber")
    .withParameter("context", &context)
    .withParameter("args", (const void*)0x1234);
  instance.publish((const uint8_t*)0x1234);
}

TEST(tiny_single_subscriber_event, should_not_publish_to_subscribers_that_have_unsubscribed)
{
  instance.subscribe(&subscription);
  instance.unsubscribe(&subscription);

  instance.publish((const uint8_t*)0x1234);
}
