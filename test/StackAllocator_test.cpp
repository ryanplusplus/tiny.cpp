/*!
 * @file
 * @brief
 */

#include <cstring>
#include "tiny/StackAllocator.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(StackAllocator)
{
  enum {
    largest_supported_size = StackAllocator::largest_supported_size
  };
};

TEST(StackAllocator, should_allocate_aligned_memory_of_the_requested_size)
{
  static size_t current_size;

  for(size_t size = 1; size <= largest_supported_size; size++) {
    current_size = size;

    StackAllocator::allocate_aligned(
      size, (void*)(0x1234), +[](void* context, void* data) {
        CHECK(context == (void*)0x1234);
        memset(data, 0xA5, current_size);
      });
  }
}