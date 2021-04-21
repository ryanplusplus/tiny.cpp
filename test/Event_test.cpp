/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny/Event.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(Event)
{
  Event<uint8_t> event;

  uint16_t context_1;
  EventSubscription<uint8_t> subscription_1{&context_1, subscriber_1};

  uint32_t context_2;
  EventSubscription<uint8_t> subscription_2{&context_2, subscriber_2};

  static void subscriber_1(uint16_t * context, uint8_t args)
  {
    mock()
      .actualCall("subscriber_1")
      .withParameter("context", context)
      .withParameter("args", args);
  }

  static void subscriber_2(uint32_t * context, uint8_t args)
  {
    mock()
      .actualCall("subscriber_2")
      .withParameter("context", context)
      .withParameter("args", args);
  }
};

TEST(Event, should_do_nothing_when_published_with_no_subscribers)
{
  event.publish((uint8_t)0x12);
}

TEST(Event, should_publish_to_all_subscribers)
{
  event.subscribe(subscription_1);
  event.subscribe(subscription_2);

  mock()
    .expectOneCall("subscriber_1")
    .withParameter("context", &context_1)
    .withParameter("args", (uint8_t)0x12);
  mock()
    .expectOneCall("subscriber_2")
    .withParameter("context", &context_2)
    .withParameter("args", (uint8_t)0x12);
  event.publish((uint8_t)0x12);
}

TEST(Event, should_not_publish_to_subscribers_that_have_unsubscribed)
{
  event.subscribe(subscription_1);
  event.subscribe(subscription_2);
  event.unsubscribe(subscription_1);

  mock()
    .expectOneCall("subscriber_2")
    .withParameter("context", &context_2)
    .withParameter("args", (uint8_t)0x56);
  event.publish((uint8_t)0x56);
}

TEST(Event, should_allow_subscribers_to_resubscribe)
{
  event.subscribe(subscription_1);
  event.subscribe(subscription_2);
  event.subscribe(subscription_1);

  mock()
    .expectOneCall("subscriber_1")
    .withParameter("context", &context_1)
    .withParameter("args", (uint8_t)0x56);
  mock()
    .expectOneCall("subscriber_2")
    .withParameter("context", &context_2)
    .withParameter("args", (uint8_t)0x56);
  event.publish((uint8_t)0x56);
}
