/*!
 * @file
 * @brief Ring/circular buffer. Usable as a FIFO queue for fixed-size elements.
 *
 * Can be used to pass data from the interrupt context to the non-interrupt
 * context if the follow constraints hold:
 * - `unsigned` is an atomic type
 * - The interrupt context only calls `insert` and `count`
 * - The non-interrupt context only calls `remove`, `at`, and `count`
 * - The ring buffer is never allowed to get full
 */

#ifndef tiny_RawRingBuffer_hpp
#define tiny_RawRingBuffer_hpp

#include <cstdbool>

namespace tiny {
  class RawRingBuffer {
   public:
    RawRingBuffer(
      void* buffer,
      unsigned element_size,
      unsigned element_count);

    RawRingBuffer(const RawRingBuffer&) = delete;

    void operator=(const RawRingBuffer& other) = delete;

   public:
    /*!
     * The number of elements the ring buffer can hold.
     */
    unsigned capacity()
    {
      return _capacity;
    }

    /*!
     * The number of elements currently stored in the ring buffer.
     */
    unsigned count();

    /*!
     * Gets the element at the specified index. If the index is larger than
     * the size then the element buffer will not be written.
     */
    void at(unsigned index, void* element);

    /*!
     * Insert an element into the ring buffer. If the ring buffer is full,
     * the oldest element will be overwritten.
     */
    void insert(const void* element);

    /*!
     * Removes the oldest element from the ring buffer and writes it into the
     * provided buffer. If the ring buffer is empty then the element will not
     * be written.
     */
    void remove(void* element);

    /*!
     * Removes all elements from the ring buffer.
     */
    void clear();

   private:
    void* buffer;
    unsigned element_size;
    volatile unsigned head{};
    volatile unsigned tail{};
    unsigned _capacity;
    bool full{};
  };
}

#endif
