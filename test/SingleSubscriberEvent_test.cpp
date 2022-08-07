/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny/SingleSubscriberEvent.hpp"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(SingleSubscriberEvent)
{
  uint16_t context{};
  SingleSubscriberEvent<uint8_t> event{};
  EventSubscription<uint8_t> subscription{ &context, subscriber };

  static void subscriber(uint16_t * context, uint8_t args)
  {
    mock()
      .actualCall("subscriber")
      .withParameter("context", context)
      .withParameter("args", args);
  }
};

TEST(SingleSubscriberEvent, should_do_nothing_when_published_with_no_subscribers)
{
  event.publish((uint8_t)0x12);
}

TEST(SingleSubscriberEvent, should_publish_to_all_subscribers)
{
  event.subscribe(subscription);

  mock()
    .expectOneCall("subscriber")
    .withParameter("context", &context)
    .withParameter("args", (uint8_t)0x12);
  event.publish((uint8_t)0x12);
}

TEST(SingleSubscriberEvent, should_not_publish_to_subscribers_that_have_unsubscribed)
{
  event.subscribe(subscription);
  event.unsubscribe(subscription);

  event.publish((uint8_t)0x12);
}
