/*!
 * @file
 * @brief
 */

#include "tiny/StackAllocator.hpp"
#include "tiny/utils.h"

using namespace tiny;

void StackAllocator::_allocate_aligned(
  size_t size,
  void* context,
  Callback callback)
{
  for(auto worker : workers) {
    if(size <= worker.size) {
      worker.worker(callback, context);
      return;
    }
  }
}
