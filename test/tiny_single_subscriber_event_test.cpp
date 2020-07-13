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
  SingleSubscriberEvent<uint8_t> event;
  EventSubscription<uint8_t> subscription{&context, subscriber};

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
  event.publish((const uint8_t*)0x1234);
}

TEST(tiny_single_subscriber_event, should_publish_to_all_subscribers)
{
  event.subscribe(&subscription);

  mock()
    .expectOneCall("subscriber")
    .withParameter("context", &context)
    .withParameter("args", (const void*)0x1234);
  event.publish((const uint8_t*)0x1234);
}

TEST(tiny_single_subscriber_event, should_not_publish_to_subscribers_that_have_unsubscribed)
{
  event.subscribe(&subscription);
  event.unsubscribe(&subscription);

  event.publish((const uint8_t*)0x1234);
}
