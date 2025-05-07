/*!
 * @file
 * @brief
 *
 * In order to fulfill the interrupt-safety guarantees made in the header:
 * - insert must only write to the head pointer when the ring buffer is not full
 * - remove must only write to the tail pointer
 * - remove must write to the tail pointer after copying the element
 */

#include <cstdint>
#include <cstring>
#include "tiny/RawRingBuffer.hpp"

using namespace tiny;

static inline unsigned increment_with_wrap(unsigned x, unsigned limit)
{
  if(++x >= limit) {
    x = 0;
  }
  return x;
}

RawRingBuffer::RawRingBuffer(
  void* buffer,
  unsigned element_size,
  unsigned element_count)
  : buffer{ buffer },
    element_size{ element_size },
    _capacity{ element_count }
{
}

unsigned RawRingBuffer::count() const
{
  if(full) {
    return _capacity;
  }
  else {
    auto count = head - tail;

    if(count > _capacity) {
      count += _capacity;
    }

    return count;
  }
}

void RawRingBuffer::at(unsigned index, void* element) const
{
  auto buffer_index = (tail + index);
  if(buffer_index >= _capacity) {
    buffer_index -= _capacity;
  }

  auto source = reinterpret_cast<uint8_t*>(buffer) + buffer_index * element_size;
  memcpy(element, source, element_size);
}

void RawRingBuffer::insert(const void* element)
{
  auto initial_head = head;
  auto initial_tail = tail;
  auto destination = reinterpret_cast<uint8_t*>(buffer) + initial_head * element_size;
  memcpy(destination, element, element_size);

  if(initial_head == initial_tail && full) {
    tail = increment_with_wrap(initial_tail, _capacity);
  }

  auto new_head = increment_with_wrap(initial_head, _capacity);
  head = new_head;

  if(new_head == tail) {
    full = true;
  }
}

void RawRingBuffer::remove(void* element)
{
  auto initial_tail = tail;

  if(head != initial_tail || full) {
    auto source = reinterpret_cast<uint8_t*>(buffer) + initial_tail * element_size;
    memcpy(element, source, element_size);

    tail = increment_with_wrap(initial_tail, _capacity);
    full = false;
  }
}

void RawRingBuffer::clear()
{
  head = 0;
  tail = 0;
  full = false;
}
