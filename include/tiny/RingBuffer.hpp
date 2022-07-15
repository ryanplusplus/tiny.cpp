/*!
 * @file
 * @brief Wrapper for RawRingBuffer that is type safe and allocates its own buffer.
 */

#ifndef tiny_RingBuffer_hpp
#define tiny_RingBuffer_hpp

#include <array>
#include "tiny/RawRingBuffer.hpp"

namespace tiny {
  template <typename T, unsigned element_count>
  class RingBuffer {
   public:
    RingBuffer()
      : buffer{}, ring_buffer{&buffer, sizeof(T), element_count}
    {
    }

    RingBuffer(const RingBuffer&) = delete;

    void operator=(const RingBuffer& other) = delete;

   public:
    /*!
     * The number of elements the ring buffer can hold.
     */
    unsigned capacity()
    {
      return ring_buffer.capacity();
    }

    /*!
     * The number of elements currently stored in the ring buffer.
     */
    unsigned count()
    {
      return ring_buffer.count();
    }

    /*!
     * Gets the element at the specified index. If the index is larger than
     * the size then the element buffer will not be written.
     */
    T at(unsigned index)
    {
      T element;
      ring_buffer.at(index, &element);
      return element;
    }

    /*!
     * Insert an element into the ring buffer. If the ring buffer is full,
     * the oldest element will be overwritten.
     */
    void insert(T& element)
    {
      ring_buffer.insert(&element);
    }

    /*!
     * Insert an element into the ring buffer. If the ring buffer is full,
     * the oldest element will be overwritten.
     */
    void insert(T&& element)
    {
      insert(element);
    }

    /*!
     * Removes the oldest element from the ring buffer and writes it into the
     * provided buffer. If the ring buffer is empty then the element will not
     * be written.
     */
    T remove()
    {
      T element;
      ring_buffer.remove(&element);
      return element;
    }

    /*!
     * Removes all elements from the ring buffer.
     */
    void clear()
    {
      ring_buffer.clear();
    }

   private:
    std::array<T, element_count> buffer;
    RawRingBuffer ring_buffer;
  };
}

#endif
