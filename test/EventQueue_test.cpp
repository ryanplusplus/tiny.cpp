/*!
 * @file
 * @brief
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny/EventQueue.hpp"

using namespace tiny;
using namespace std;

TEST_GROUP(EventQueue)
{
  unique_ptr<EventQueue> event_queue{};

  uint8_t buffer[1024];

  bool run_result;

  void setup()
  {
    mock().strictOrder();
  }

  static void callback()
  {
    mock().actualCall("callback");
  }

  static void u16_callback(const void* data)
  {
    CHECK_EQUAL(0, reinterpret_cast<uintptr_t>(data) % sizeof(uint16_t));
    mock().actualCall("u16_callback").withParameter("data", *reinterpret_cast<const uint16_t*>(data));
  }

  static void u32_callback(const void* data)
  {
    CHECK_EQUAL(0, reinterpret_cast<uintptr_t>(data) % sizeof(uint32_t));
    mock().actualCall("u32_callback").withParameter("data", *reinterpret_cast<const uint32_t*>(data));
  }

  static void unable_to_enqueue()
  {
    mock().actualCall("unable_to_enqueue");
  }

  void given_that_the_queue_has_been_initialized_with_buffer_size(unsigned buffer_size)
  {
    event_queue = make_unique<EventQueue>(buffer, buffer_size, unable_to_enqueue);
  }

  void given_that_a_u32_event_has_been_enqueued(uint32_t data)
  {
    when_a_u32_event_is_enqueued(data);
  }

  void when_a_u32_event_is_enqueued(uint32_t data)
  {
    event_queue->enqueue(u32_callback, &data, sizeof(data));
  }

  void given_that_a_u16_event_has_been_enqueued(uint16_t data)
  {
    when_a_u16_event_is_enqueued(data);
  }

  void when_a_u16_event_is_enqueued(uint16_t data)
  {
    event_queue->enqueue(u16_callback, &data, sizeof(data));
  }

  void given_that_an_event_has_been_enqueued()
  {
    when_an_event_is_enqueued();
  }

  void when_an_event_is_enqueued()
  {
    event_queue->enqueue(callback);
  }

  void u32_event_should_be_raised(uint32_t data)
  {
    mock().expectOneCall("u32_callback").withParameter("data", data);
  }

  void u16_event_should_be_raised(uint16_t data)
  {
    mock().expectOneCall("u16_callback").withParameter("data", data);
  }

  void event_should_be_raised()
  {
    mock().expectOneCall("callback");
  }

  void given_the_queue_has_been_run()
  {
    mock().disable();
    when_the_queue_is_run();
    mock().enable();
  }

  void when_the_queue_is_run()
  {
    run_result = event_queue->run();
  }

  void and_the_run_result_should_be(bool expected)
  {
    CHECK_EQUAL(expected, run_result);
  }

  void should_raise_unable_to_enqueue_error()
  {
    mock().expectOneCall("unable_to_enqueue");
  }

  void nothing_should_happen()
  {
  }
};

TEST(EventQueue, should_do_nothing_when_run_with_no_events_queued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);

  nothing_should_happen();
  when_the_queue_is_run();
  and_the_run_result_should_be(false);
}

TEST(EventQueue, should_raise_the_enqueued_event_when_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_an_event_has_been_enqueued();

  event_should_be_raised();
  when_the_queue_is_run();
  and_the_run_result_should_be(true);
}

TEST(EventQueue, should_raise_the_enqueued_event_with_data_when_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_a_u32_event_has_been_enqueued(0x12345678);

  u32_event_should_be_raised(0x12345678);
  when_the_queue_is_run();
  and_the_run_result_should_be(true);
}

TEST(EventQueue, should_enqueue_and_process_multiple_events)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(100);
  given_that_an_event_has_been_enqueued();
  given_that_a_u16_event_has_been_enqueued(0xABCD);

  event_should_be_raised();
  when_the_queue_is_run();

  u16_event_should_be_raised(0xABCD);
  when_the_queue_is_run();
}

TEST(EventQueue, should_ensure_that_events_can_be_enqueued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::Callback) + 1);
  nothing_should_happen();
  when_an_event_is_enqueued();

  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::Callback));
  should_raise_unable_to_enqueue_error();
  when_an_event_is_enqueued();
}

TEST(EventQueue, should_ensure_that_events_are_fully_remove_from_the_queue_after_being_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::Callback) + 1);
  given_that_an_event_has_been_enqueued();
  given_the_queue_has_been_run();

  nothing_should_happen();
  when_an_event_is_enqueued();
}

TEST(EventQueue, should_ensure_that_events_with_data_can_be_enqueued)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::CallbackWithData) + sizeof(uint8_t) + sizeof(uint16_t) + 1);
  nothing_should_happen();
  when_a_u16_event_is_enqueued(0x1234);

  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::CallbackWithData) + sizeof(uint8_t) + sizeof(uint16_t));
  should_raise_unable_to_enqueue_error();
  when_a_u16_event_is_enqueued(0x1234);
}

TEST(EventQueue, should_ensure_that_events_with_data_are_fully_remove_from_the_queue_after_being_run)
{
  given_that_the_queue_has_been_initialized_with_buffer_size(sizeof(uint8_t) + sizeof(IEventQueue::CallbackWithData) + sizeof(uint8_t) + sizeof(uint16_t) + 1);
  given_that_a_u16_event_has_been_enqueued(0x1234);
  given_the_queue_has_been_run();

  nothing_should_happen();
  when_a_u16_event_is_enqueued(0x1234);
}
