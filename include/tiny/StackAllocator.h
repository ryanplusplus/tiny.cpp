/*!
 * @file
 * @brief
 */

#ifndef StackAllocator_h
#define StackAllocator_h

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

    //  private:
    typedef void (*Callback)(void* context, void* data);

   private:
    static auto _allocate_aligned(
      size_t size,
      void* context,
      Callback callback)
      -> void;
  };
}

#endif
