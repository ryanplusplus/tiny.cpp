/*!
 * @file
 * @brief FIFO queue for arbitrarily-sized elements.
 */

#ifndef tiny_Queue_hpp
#define tiny_Queue_hpp

#include <stddef.h>
#include <stdint.h>
#include "tiny/RawRingBuffer.hpp"

namespace tiny {
  class Queue {
   public:
    Queue(void* buffer, unsigned buffer_size)
      : element_count(0), ring_buffer(buffer, sizeof(uint8_t), buffer_size)
    {
    }

   public:
    bool enqueue(const void* element, uint16_t size);
    void dequeue(void* element, uint16_t* size);
    void discard();
    void peek(void* element, uint16_t* size, uint16_t index);
    void peek_partial(void* element, uint16_t size_limit, uint16_t index);
    void peek_size(uint16_t* size, uint16_t index);
    uint16_t count();

   private:
    void insert_element_size(uint16_t element_size);
    void insert_element(const void* element, uint16_t element_size);
    void remove_element_size(uint16_t* sizePlaceHolder);
    void remove_element(void* element, uint16_t element_size);
    void discard_element(uint16_t element_size);
    void peek_element_size_at_ring_buffer_index(uint16_t* element_size, uint16_t location);
    uint16_t ring_buffer_index_for_element_index(uint16_t element_index);
    void peek_element_at_ring_buffer_index(void* element, uint16_t element_size, uint16_t location);

   private:
    uint16_t element_count;
    RawRingBuffer ring_buffer;
  };
}

#endif
