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

#ifndef tiny_RingBuffer_h
#define tiny_RingBuffer_h

#include <cstdbool>

namespace tiny {
  class RingBuffer {
   public:
    RingBuffer(
      void* buffer,
      unsigned element_size,
      unsigned element_count);

    auto operator=(const RingBuffer& other) -> void = delete;

   public:
    auto capacity() -> unsigned
    {
      return _capacity;
    }

    auto count() -> unsigned;
    auto at(unsigned index, void* element) -> void;
    auto insert(const void* element) -> void;
    auto remove(void* element) -> void;
    auto clear() -> void;

   private:
    void* buffer;
    unsigned element_size;
    volatile unsigned head;
    volatile unsigned tail;
    unsigned _capacity;
    bool full;
  };
}

#endif
