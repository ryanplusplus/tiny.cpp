/*!
 * @file
 * @brief
 */

#ifndef StackAllocator_h
#define StackAllocator_h

#include <cstdint>
#include <cstddef>

namespace tiny {
  class StackAllocator {
   public:
    static constexpr size_t largest_supported_size = 256;

   public:
    template <typename Context>
    static auto allocate_aligned(
      size_t size,
      Context* context,
      void (*callback)(Context* context, void* data))
      -> void
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
    static auto
      _allocate_aligned(
        size_t size,
        void* context,
        Callback callback)
        -> void;
  };
}

#endif
