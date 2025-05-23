/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny/RawRingBuffer.hpp"

using namespace tiny;
using namespace std;

TEST_GROUP(RawRingBuffer)
{
  unique_ptr<RawRingBuffer> ring_buffer{};
  uint8_t buffer[1024];

  void given_initialization_with_element_size_and_count(
    unsigned element_size,
    unsigned element_count)
  {
    ring_buffer = make_unique<RawRingBuffer>(buffer, element_size, element_count);
  }

  void after_clearing()
  {
    ring_buffer->clear();
  }

  void the_count_should_be(unsigned expected)
  {
    CHECK_EQUAL(expected, ring_buffer->count());
  }

  template <typename T>
  void after_inserting_element(T element)
  {
    ring_buffer->insert(&element);
  }

  template <typename T>
  void after_removing_element()
  {
    T element;
    ring_buffer->remove(&element);
  }

  template <typename T>
  void should_remove_element(T expected)
  {
    T actual;
    ring_buffer->remove(&actual);
    CHECK_EQUAL(expected, actual);
  }

  template <typename T>
  void the_element_at_index_should_be(unsigned index, T expected)
  {
    T actual;
    ring_buffer->at(index, &actual);
    CHECK_EQUAL(expected, actual);
  }
};

TEST(RawRingBuffer, should_be_empty_after_initialization)
{
  given_initialization_with_element_size_and_count(10, 20);
  the_count_should_be(0);
}

TEST(RawRingBuffer, should_update_count_when_inserting_elements)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 5);

  after_inserting_element<uint16_t>(1234);
  the_count_should_be(1);

  after_inserting_element<uint16_t>(4321);
  the_count_should_be(2);
}

TEST(RawRingBuffer, should_give_the_correct_count_when_full)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 3);

  after_inserting_element<uint16_t>(111);
  after_inserting_element<uint16_t>(222);
  after_inserting_element<uint16_t>(333);
  the_count_should_be(3);
}

TEST(RawRingBuffer, should_give_the_correct_count_after_having_been_when_full)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 3);

  after_inserting_element<uint16_t>(111);
  after_inserting_element<uint16_t>(222);
  after_inserting_element<uint16_t>(333);
  after_removing_element<uint16_t>();
  after_removing_element<uint16_t>();
  the_count_should_be(1);
}

TEST(RawRingBuffer, should_give_the_correct_count_after_overwriting)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 3);

  after_inserting_element<uint16_t>(111);
  after_inserting_element<uint16_t>(222);
  after_inserting_element<uint16_t>(333);
  after_inserting_element<uint16_t>(444);
  the_count_should_be(3);
}

TEST(RawRingBuffer, should_update_count_when_removing_elements)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 5);

  after_inserting_element<uint16_t>(1234);
  after_inserting_element<uint16_t>(4321);

  after_removing_element<uint16_t>();
  the_count_should_be(1);

  after_removing_element<uint16_t>();
  the_count_should_be(0);
}

TEST(RawRingBuffer, should_give_the_correct_count_after_head_wraps_around)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 3);

  after_inserting_element<uint16_t>(111);
  after_inserting_element<uint16_t>(222);
  after_inserting_element<uint16_t>(333);
  after_removing_element<uint16_t>();
  after_inserting_element<uint16_t>(444);
  the_count_should_be(3);
}

TEST(RawRingBuffer, should_be_empty_after_clearing)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 5);

  after_inserting_element<uint16_t>(1234);
  after_inserting_element<uint16_t>(4321);

  after_clearing();
  the_count_should_be(0);
}

TEST(RawRingBuffer, should_do_nothing_when_removing_when_empty)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 5);
  after_removing_element<uint16_t>();
  the_count_should_be(0);
}

TEST(RawRingBuffer, should_remove_elements_in_fifo_order)
{
  given_initialization_with_element_size_and_count(sizeof(uint16_t), 5);

  after_inserting_element<uint16_t>(111);
  after_inserting_element<uint16_t>(222);
  after_inserting_element<uint16_t>(333);

  should_remove_element<uint16_t>(111);
  should_remove_element<uint16_t>(222);
  should_remove_element<uint16_t>(333);
}

TEST(RawRingBuffer, should_overwrite_the_oldest_element_when_full)
{
  given_initialization_with_element_size_and_count(sizeof(uint8_t), 3);

  after_inserting_element<uint8_t>(11);
  after_inserting_element<uint8_t>(22);
  after_inserting_element<uint8_t>(33);
  after_inserting_element<uint8_t>(44);
  after_inserting_element<uint8_t>(55);

  should_remove_element<uint8_t>(33);
  should_remove_element<uint8_t>(44);
  should_remove_element<uint8_t>(55);
}

TEST(RawRingBuffer, should_allow_elements_to_be_accessed_by_index)
{
  given_initialization_with_element_size_and_count(sizeof(uint32_t), 3);

  after_inserting_element<uint32_t>(110000);
  after_inserting_element<uint32_t>(220000);
  after_inserting_element<uint32_t>(330000);
  after_inserting_element<uint32_t>(440000);
  after_inserting_element<uint32_t>(550000);

  the_element_at_index_should_be<uint32_t>(0, 330000);
  the_element_at_index_should_be<uint32_t>(1, 440000);
  the_element_at_index_should_be<uint32_t>(2, 550000);
}
