/*!
 * @file
 * @brief
 */

#include "tiny/StackAllocator.hpp"

using namespace tiny;

typedef void (*Callback)(void* context, void* data);

template <size_t size>
static void worker(Callback callback, void* context)
{
  constexpr auto max_align = sizeof(std::max_align_t);
  std::max_align_t data[(size > max_align ? size : max_align) / max_align];
  callback(context, data);
}

struct Worker {
  size_t size;
  void (*worker)(Callback callback, void* context);
};

static constexpr Worker workers[] = {
  { 8, worker<8> },
  { 16, worker<16> },
  { 32, worker<32> },
  { 64, worker<64> },
  { 128, worker<128> },
  { 256, worker<256> },
};

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
