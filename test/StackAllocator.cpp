/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny/StackAllocator.h"
#include "tiny/utils.h"

using namespace tiny;

#define define_worker(_size)                                                   \
  static void worker_##_size(StackAllocator::Callback callback, void* context) \
  {                                                                            \
    uint64_t data[_size / sizeof(uint64_t)];                                   \
    callback(context, data);                                                   \
  }                                                                            \
  typedef int dummy##_size

define_worker(8);
define_worker(16);
define_worker(32);
define_worker(64);
define_worker(128);
define_worker(256);

struct Worker {
  size_t size;
  void (*worker)(StackAllocator::Callback callback, void* context);
};

static const Worker workers[] = {
  {8, worker_8},
  {16, worker_16},
  {32, worker_32},
  {64, worker_64},
  {128, worker_128},
  {256, worker_256},
};

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
