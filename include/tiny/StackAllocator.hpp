/*!
 * @file
 * @brief Allows for stack data to be dynamically allocated without alloca() or VLAs.
 */

#ifndef tiny_StackAllocator_hpp
#define tiny_StackAllocator_hpp

#include <cstdint>
#include <cstddef>

namespace tiny {
  class StackAllocator {
   public:
    static constexpr size_t largest_supported_size = 256;

   public:
    /*!
     * Invokes the provided callback to an aligned pointer of the requested size.
     *
     * @warning The allocated data is valid only during the callback.
     */
    template <typename Context>
    static void allocate_aligned(
      size_t size,
      Context* context,
      void (*callback)(Context* context, void* data))
    {
      _allocate_aligned(size, context, reinterpret_cast<Callback>(callback));
    }

   private:
    typedef void (*Callback)(void* context, void* data);

    template <size_t size>
    static void worker(StackAllocator::Callback callback, void* context)
    {
      uint64_t data[size / sizeof(uint64_t)];
      callback(context, data);
    }

    struct Worker {
      size_t size;
      void (*worker)(StackAllocator::Callback callback, void* context);
    };

    static constexpr Worker workers[] = {
      {8, worker<8>},
      {16, worker<16>},
      {32, worker<32>},
      {64, worker<64>},
      {128, worker<128>},
      {256, worker<256>},
    };

   private:
    static void _allocate_aligned(
      size_t size,
      void* context,
      Callback callback);
  };
}

#endif
