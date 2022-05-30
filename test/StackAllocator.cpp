/*!
 * @file
 * @brief
 */

#include "tiny/StackAllocator.h"
#include "tiny/utils.h"

using namespace tiny;

auto StackAllocator::_allocate_aligned(
  size_t size,
  void* context,
  Callback callback)
  -> void
{
  for(auto worker : workers) {
    if(size <= worker.size) {
      worker.worker(callback, context);
      return;
    }
  }
}
