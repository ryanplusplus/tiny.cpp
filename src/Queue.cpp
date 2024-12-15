/*!
 * @file
 * @brief
 */

#include "tiny/Queue.hpp"

void tiny::Queue::insert_element_size(uint16_t element_size)
{
  uint8_t* destination = (uint8_t*)&element_size;
  for(uint8_t i = 0; i < sizeof(uint16_t); i++) {
    ring_buffer.insert(destination);
    destination++;
  }
}

void tiny::Queue::insert_element(const void* element, uint16_t element_size)
{
  auto source = reinterpret_cast<const uint8_t*>(element);
  for(auto i = 0; i < element_size; i++) {
    ring_buffer.insert(source);
    source++;
  }
}

void tiny::Queue::remove_element_size(uint16_t* size_placeholder)
{
  auto destination = reinterpret_cast<uint8_t*>(size_placeholder);
  for(uint8_t i = 0; i < sizeof(uint16_t); i++) {
    ring_buffer.remove(destination);
    destination++;
  }
}

void tiny::Queue::remove_element(void* element, uint16_t element_size)
{
  auto destination = reinterpret_cast<uint8_t*>(element);
  for(uint32_t i = 0; i < element_size; i++) {
    ring_buffer.remove(destination);
    destination++;
  }
}

void tiny::Queue::discard_element(uint16_t element_size)
{
  for(auto i = 0; i < element_size; i++) {
    uint8_t discard;
    ring_buffer.remove(&discard);
  }
}

void tiny::Queue::peek_element_size_at_ring_buffer_index(uint16_t* element_size, uint16_t location)
{
  auto destination = (uint8_t*)element_size;
  for(auto i = location; i < location + sizeof(uint16_t); i++) {
    ring_buffer.at(i, destination);
    destination++;
  }
}

uint16_t tiny::Queue::ring_buffer_index_for_element_index(uint16_t element_index)
{
  uint16_t element_size;
  uint16_t location = 0;
  for(auto i = 0; i < element_index; i++) {
    peek_element_size_at_ring_buffer_index(&element_size, location);
    location += (uint16_t)(element_size + sizeof(uint16_t));
  }
  return location;
}

void tiny::Queue::peek_element_at_ring_buffer_index(void* element, uint16_t element_size, uint16_t location)
{
  auto destination = reinterpret_cast<uint8_t*>(element);
  for(auto i = location; i < location + element_size; i++) {
    ring_buffer.at(i, destination);
    destination++;
  }
}

bool tiny::Queue::enqueue(const void* element, uint16_t element_size)
{
  auto required_space = element_size + sizeof(uint16_t);
  auto available_space = ring_buffer.capacity() - ring_buffer.count();

  if(required_space <= available_space) {
    insert_element_size(element_size);
    insert_element(element, element_size);
    element_count++;
    return true;
  }
  else {
    return false;
  }
}

void tiny::Queue::dequeue(void* element, uint16_t* size_storage)
{
  remove_element_size(size_storage);
  remove_element(element, *size_storage);
  element_count--;
}

void tiny::Queue::discard()
{
  uint16_t size_storage;
  remove_element_size(&size_storage);
  discard_element(size_storage);
  element_count--;
}

void tiny::Queue::peek(void* element, uint16_t* size_storage, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(element_index);
  peek_element_size_at_ring_buffer_index(size_storage, i);
  peek_element_at_ring_buffer_index(element, *size_storage, i + sizeof(uint16_t));
}

void tiny::Queue::peek_partial(void* element, uint16_t size, uint16_t offset, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(element_index);
  peek_element_at_ring_buffer_index(element, size, (uint16_t)(i + offset + sizeof(uint16_t)));
}

void tiny::Queue::peek_size(uint16_t* size_storage, uint16_t element_index)
{
  uint16_t i = ring_buffer_index_for_element_index(element_index);
  peek_element_size_at_ring_buffer_index(size_storage, i);
}

uint16_t tiny::Queue::count()
{
  return element_count;
}
