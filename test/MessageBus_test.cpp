/*!
 * @file
 * @brief
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny/MessageBus.hpp"

using namespace tiny;

TEST_GROUP(MessageBus)
{
  MessageBus bus{};

  EventSubscription<MessageBus::Message, const void*> subscription_1{ static_cast<void*>(nullptr), subscriber_1 };
  EventSubscription<MessageBus::Message, const void*> subscription_2{ static_cast<void*>(nullptr), subscriber_2 };

  static void subscriber_1(void* context, MessageBus::Message message, const void* data)
  {
    static_cast<void>(context);
    mock()
      .actualCall("subscriber_1")
      .withParameter("message", message)
      .withParameter("data", data);
  }

  static void subscriber_2(void* context, MessageBus::Message message, const void* data)
  {
    static_cast<void>(context);
    mock()
      .actualCall("subscriber_2")
      .withParameter("message", message)
      .withParameter("data", data);
  }
};

TEST(MessageBus, should_send_messages_to_all_subscribers)
{
  bus.on_receive().subscribe(subscription_1);
  bus.on_receive().subscribe(subscription_2);

  mock()
    .expectOneCall("subscriber_1")
    .withParameter("message", 123)
    .withParameter("data", reinterpret_cast<const void*>(0x1234));
  mock()
    .expectOneCall("subscriber_2")
    .withParameter("message", 123)
    .withParameter("data", reinterpret_cast<const void*>(0x1234));

  bus.send(123, reinterpret_cast<const void*>(0x1234));
}
