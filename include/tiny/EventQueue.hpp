/*!
 * @file
 * @brief Event queue that stores queued events in a ring buffer.
 *
 * This implementation is interrupt-safe provided that events are
 * queued in an interrupt context and dequeued in a non-interrupt
 * context. Queueing of one event must be completed before the next
 * event can be queued so interrupt safety is broken if a high
 * priority interrupt queues while interrupting a low priority
 * interrupt that is also queueing.
 */

#ifndef tiny_EventQueue_hpp
#define tiny_EventQueue_hpp

#include "tiny/IEventQueue.hpp"
#include "tiny/RingBuffer.hpp"

namespace tiny {
  class EventQueue final : public IEventQueue {
   public:
    typedef void (*UnableToQueueCallback)(void);

   public:
    EventQueue(
      void* buffer,
      unsigned buffer_size,
      UnableToQueueCallback unable_to_queue_callback);

    void operator=(const EventQueue& other) = delete;

    bool run();

    void enqueue(Callback callback) override;
    void enqueue(CallbackWithData callback, const void* data, uint8_t dataSize) override;

   private:
    void write_to_buffer(const void* _data, uint8_t data_size);
    void read_from_buffer(void* _data, uint8_t data_size);
    void drop_from_buffer(uint8_t count);
    uint8_t peek();
    void process_event();
    void process_event_with_data();

   private:
    RingBuffer ring_buffer;
    UnableToQueueCallback unable_to_queue_callback;
  };
}

#endif
