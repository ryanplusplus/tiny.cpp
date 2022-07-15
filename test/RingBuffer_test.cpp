/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny/RingBuffer.hpp"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;
using namespace std;

TEST_GROUP(RingBuffer)
{
  unique_ptr<RingBuffer<uint32_t, 3>> ring_buffer{};

  void given_that_the_ring_buffer_has_been_initialized()
  {
    ring_buffer = make_unique<RingBuffer<uint32_t, 3>>();
  }

  void after_clearing()
  {
    ring_buffer->clear();
  }

  void the_count_should_be(unsigned expected)
  {
    CHECK_EQUAL(expected, ring_buffer->count());
  }

  void after_inserting_element(uint32_t element)
  {
    ring_buffer->insert(element);
  }

  void after_removing_element()
  {
    ring_buffer->remove();
  }

  void should_remove_element(uint32_t expected)
  {
    auto actual = ring_buffer->remove();
    CHECK_EQUAL(expected, actual);
  }

  void the_element_at_index_should_be(unsigned index, uint32_t expected)
  {
    auto actual = ring_buffer->at(index);
    CHECK_EQUAL(expected, actual);
  }
};

TEST(RingBuffer, should_be_empty_after_initialization)
{
  given_that_the_ring_buffer_has_been_initialized();
  the_count_should_be(0);
}

TEST(RingBuffer, should_update_count_when_inserting_elements)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(1234);
  the_count_should_be(1);

  after_inserting_element(4321);
  the_count_should_be(2);
}

TEST(RingBuffer, should_give_the_correct_size_when_full)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(111);
  after_inserting_element(222);
  after_inserting_element(333);
  the_count_should_be(3);
}

TEST(RingBuffer, should_give_the_correct_size_after_overwriting)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(111);
  after_inserting_element(222);
  after_inserting_element(333);
  after_inserting_element(444);
  the_count_should_be(3);
}

TEST(RingBuffer, should_update_size_when_removing_elements)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(1234);
  after_inserting_element(4321);

  after_removing_element();
  the_count_should_be(1);

  after_removing_element();
  the_count_should_be(0);
}

TEST(RingBuffer, should_give_the_correct_size_after_head_wraps_around)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(111);
  after_inserting_element(222);
  after_inserting_element(333);
  after_removing_element();
  after_inserting_element(444);
  the_count_should_be(3);
}

TEST(RingBuffer, should_be_empty_after_clearing)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(1234);
  after_inserting_element(4321);

  after_clearing();
  the_count_should_be(0);
}

TEST(RingBuffer, should_do_nothing_when_removing_when_empty)
{
  given_that_the_ring_buffer_has_been_initialized();
  after_removing_element();
  the_count_should_be(0);
}

TEST(RingBuffer, should_remove_elements_in_fifo_order)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(111);
  after_inserting_element(222);
  after_inserting_element(333);

  should_remove_element(111);
  should_remove_element(222);
  should_remove_element(333);
}

TEST(RingBuffer, should_overwrite_the_oldest_element_when_full)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(11);
  after_inserting_element(22);
  after_inserting_element(33);
  after_inserting_element(44);
  after_inserting_element(55);

  should_remove_element(33);
  should_remove_element(44);
  should_remove_element(55);
}

TEST(RingBuffer, should_allow_elements_to_be_accessed_by_index)
{
  given_that_the_ring_buffer_has_been_initialized();

  after_inserting_element(110000);
  after_inserting_element(220000);
  after_inserting_element(330000);
  after_inserting_element(440000);
  after_inserting_element(550000);

  the_element_at_index_should_be(0, 330000);
  the_element_at_index_should_be(1, 440000);
  the_element_at_index_should_be(2, 550000);
}

TEST(RingBuffer, should_allow_rvalues_to_be_inserted)
{
  given_that_the_ring_buffer_has_been_initialized();

  ring_buffer->insert(5);
  the_element_at_index_should_be(0, 5);
}
