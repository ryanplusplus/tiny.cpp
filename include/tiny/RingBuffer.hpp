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

#ifndef tiny_RingBuffer_hpp
#define tiny_RingBuffer_hpp

#include <cstdbool>

namespace tiny {
  class RingBuffer {
   public:
    RingBuffer(
      void* buffer,
      unsigned element_size,
      unsigned element_count);

    RingBuffer(const RingBuffer&) = delete;

    void operator=(const RingBuffer& other) = delete;

   public:
    unsigned capacity()
    {
      return _capacity;
    }

    unsigned count();
    void at(unsigned index, void* element);
    void insert(const void* element);
    void remove(void* element);
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
