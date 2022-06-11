/*!
 * @file
 * @brief
 *
 * In order to fulfill the interrupt-safety guarantees made in the header:
 * - insert must only write to the head pointer when the ring buffer is not full
 * - remove must only write to the tail pointer
 * - remove must write to the tail pointer after copying the element
 */

#include <cstring>
#include <cstdint>
#include "tiny/RingBuffer.hpp"

using namespace tiny;

RingBuffer::RingBuffer(
  void* buffer,
  unsigned element_size,
  unsigned element_count)
  : buffer{buffer},
    element_size{element_size},
    _capacity{element_count}
{
}

auto RingBuffer::count() -> unsigned
{
  if(full) {
    return _capacity;
  }
  else {
    return (head - tail) % _capacity;
  }
}

auto RingBuffer::at(unsigned index, void* element) -> void
{
  auto buffer_index = (tail + index) % _capacity;
  auto source = reinterpret_cast<uint8_t*>(buffer) + buffer_index * element_size;
  memcpy(element, source, element_size);
}

auto RingBuffer::insert(const void* element) -> void
{
  auto destination = reinterpret_cast<uint8_t*>(buffer) + head * element_size;
  memcpy(destination, element, element_size);

  if(head == tail && full) {
    tail = tail + 1;
    if(tail == _capacity) {
      tail = 0;
    }
  }

  head = head + 1;
  if(head == _capacity) {
    head = 0;
  }

  if(head == tail) {
    full = true;
  }
}

auto RingBuffer::remove(void* element) -> void
{
  if(head != tail || full) {
    auto source = reinterpret_cast<uint8_t*>(buffer) + tail * element_size;
    memcpy(element, source, element_size);

    tail = tail + 1;
    if(tail == _capacity) {
      tail = 0;
    }
  }
}

auto RingBuffer::clear() -> void
{
  head = 0;
  tail = 0;
  full = false;
}
