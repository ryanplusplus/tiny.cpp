/*!
 * @file
 * @brief
 */

#include "tiny/EventQueue.hpp"
#include "tiny/StackAllocator.hpp"

using namespace tiny;

enum class event_type : uint8_t {
  event,
  event_with_data,
};

enum {
  event_overhead =
    sizeof(uint8_t) + // start
    sizeof(EventQueue::Callback), // callback

  event_with_data_overhead =
    sizeof(uint8_t) + // start
    sizeof(EventQueue::CallbackWithData) + // callback
    sizeof(uint8_t) // data size
};

static constexpr auto event_type_value = event_type::event;
static constexpr auto event_with_data_type_value = event_type::event_with_data;

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

void EventQueue::enqueue(Callback callback)
{
  auto capacity = ring_buffer.capacity();
  auto count = ring_buffer.count();
  auto event_size = event_overhead;

  if((capacity - count) <= event_size) {
    unable_to_queue_callback();
    return;
  }

  write_to_buffer(&event_type_value, sizeof(event_type_value));
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

  write_to_buffer(&event_with_data_type_value, sizeof(event_with_data_type_value));
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

  read_from_buffer(&context.callback, sizeof(context.callback));
  read_from_buffer(&context.data_size, sizeof(context.data_size));

  StackAllocator::allocate_aligned(
    context.data_size, &context, +[](Context* context, void* data) {
      context->self->read_from_buffer(data, context->data_size);
      context->callback(data);
    });
}

bool EventQueue::run()
{
  if(ring_buffer.count() == 0) {
    return false;
  }

  event_type type;
  read_from_buffer(&type, sizeof(type));

  switch(type) {
    case event_type::event:
      process_event();
      break;

    case event_type::event_with_data:
      process_event_with_data();
      break;
  }

  return true;
}
