/*!
 * @file
 * @brief
 */

#include "tiny_MessageBus.h"
#include "tiny_utils.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(tiny_MessageBus)
{
  MessageBus bus;

  EventSubscription<MessageBus::OnReceiveArgs> subscription_1{(void*)nullptr, subscriber_1};
  EventSubscription<MessageBus::OnReceiveArgs> subscription_2{(void*)nullptr, subscriber_2};

  static void subscriber_1(void* context, const MessageBus::OnReceiveArgs* args)
  {
    (void)context;
    mock()
      .actualCall("subscriber_1")
      .withParameter("message", args->message)
      .withParameter("data", args->data);
  }

  static void subscriber_2(void* context, const MessageBus::OnReceiveArgs* args)
  {
    (void)context;
    mock()
      .actualCall("subscriber_2")
      .withParameter("message", args->message)
      .withParameter("data", args->data);
  }
};

TEST(tiny_MessageBus, should_send_messages_to_all_subscribers)
{
  bus.on_receive()->subscribe(&subscription_1);
  bus.on_receive()->subscribe(&subscription_2);

  mock()
    .expectOneCall("subscriber_1")
    .withParameter("message", 123)
    .withParameter("data", (const void*)0x1234);
  mock()
    .expectOneCall("subscriber_2")
    .withParameter("message", 123)
    .withParameter("data", (const void*)0x1234);

  bus.send(123, (const void*)0x1234);
}
