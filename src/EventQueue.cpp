/*!
 * @file
 * @brief
 */

#include "tiny/EventQueue.hpp"
#include "tiny/StackAllocator.hpp"

using namespace tiny;

enum {
  event_start,
  event_with_data_start,

  event_overhead =
    sizeof(uint8_t) + // start
    sizeof(EventQueue::Callback), // callback

  event_with_data_overhead =
    sizeof(uint8_t) + // start
    sizeof(EventQueue::CallbackWithData) + // callback
    sizeof(uint8_t) // data size
};

static constexpr uint8_t event_start_value = event_start;
static constexpr uint8_t event_with_data_start_value = event_with_data_start;

void EventQueue::write_to_buffer(const void* _data, uint8_t data_size)
{
  auto data = reinterpret_cast<const uint8_t*>(_data);

  for(uint16_t i = 0; i < data_size; i++) {
    ring_buffer.insert(data++);
  }
}

void EventQueue::read_from_buffer(void* _data, uint8_t data_size)
{
  auto data = reinterpret_cast<uint8_t*>(_data);

  for(uint16_t i = 0; i < data_size; i++) {
    ring_buffer.remove(data++);
  }
}

void EventQueue::drop_from_buffer(uint8_t count)
{
  for(uint16_t i = 0; i < count; i++) {
    uint8_t drop;
    ring_buffer.remove(&drop);
  }
}

uint8_t EventQueue::peek()
{
  uint8_t peeked;
  ring_buffer.at(0, &peeked);
  return peeked;
}

void EventQueue::enqueue(Callback callback)
{
  auto capacity = ring_buffer.capacity();
  auto count = ring_buffer.count();
  auto event_size = event_overhead;

  if((capacity - count) <= event_size) {
    unable_to_queue_callback();
    return;
  }

  write_to_buffer(&event_start_value, sizeof(event_start_value));
  write_to_buffer(&callback, sizeof(callback));
}

void EventQueue::enqueue(
  CallbackWithData callback,
  const void* data,
  uint8_t data_size)
{
  auto capacity = ring_buffer.capacity();
  auto count = ring_buffer.count();
  auto event_size = static_cast<unsigned>(event_with_data_overhead + data_size);

  if((capacity - count) <= event_size) {
    unable_to_queue_callback();
    return;
  }

  write_to_buffer(&event_with_data_start_value, sizeof(event_with_data_start_value));
  write_to_buffer(&callback, sizeof(callback));
  write_to_buffer(&data_size, sizeof(data_size));
  write_to_buffer(data, data_size);
}

EventQueue::EventQueue(
  void* buffer,
  unsigned buffer_size,
  UnableToQueueCallback unable_to_queue_callback)
  : ring_buffer{ buffer, 1, buffer_size },
    unable_to_queue_callback{ unable_to_queue_callback }
{
}

void EventQueue::process_event()
{
  Callback callback;

  drop_from_buffer(1);
  read_from_buffer(&callback, sizeof(callback));
  callback();
}

void EventQueue::process_event_with_data()
{
  struct Context {
    EventQueue* self;
    EventQueue::CallbackWithData callback;
    uint8_t data_size;
  };

  Context context = { this, nullptr, 0 };

  drop_from_buffer(1);
  read_from_buffer(&context.callback, sizeof(context.callback));
  read_from_buffer(&context.data_size, sizeof(context.data_size));

  StackAllocator::allocate_aligned(
    context.data_size, &context, +[](Context* context, void* _data) {
      auto data = reinterpret_cast<uint8_t*>(_data);

      for(uint8_t i = 0; i < context->data_size; i++) {
        context->self->read_from_buffer(data, sizeof(*data));
        data++;
      }

      context->callback(_data);
    });

  drop_from_buffer(context.data_size);
}

bool EventQueue::run()
{
  if(ring_buffer.count() == 0) {
    return false;
  }

  switch(peek()) {
    case event_start:
      process_event();
      break;

    case event_with_data_start:
      process_event_with_data();
      break;
  }

  return true;
}
