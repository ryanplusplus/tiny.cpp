/*!
 * @file
 * @brief Allows for stack data to be dynamically allocated without alloca() or VLAs.
 */

#ifndef tiny_StackAllocator_hpp
#define tiny_StackAllocator_hpp

#include <cstddef>
#include <cstdint>

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

   private:
    static void _allocate_aligned(
      size_t size,
      void* context,
      Callback callback);
  };
}

#endif
