/*!
 * @file
 * @brief
 */

#include "tiny/List.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

TEST_GROUP(List)
{
  List list{};
  List::Node node_1{}, node_2{}, node_3{};
};

TEST(List, should_be_empty_after_init)
{
  CHECK(0 == list.count());
}

TEST(List, count_should_update_as_nodes_are_added)
{
  list.push_front(node_1);
  CHECK(1 == list.count());

  list.push_front(node_2);
  CHECK(2 == list.count());

  list.push_back(node_3);
  CHECK(3 == list.count());
}

TEST(List, should_push_and_pop_nodes_at_front)
{
  list.push_front(node_1);
  list.push_front(node_2);
  list.push_front(node_3);

  CHECK(node_3 == &list.pop_front());
  CHECK(node_2 == &list.pop_front());
  CHECK(node_1 == &list.pop_front());
}

TEST(List, should_push_and_pop_nodes_at_back)
{
  list.push_back(node_1);
  list.push_back(node_2);
  list.push_back(node_3);

  CHECK(node_3 == &list.pop_back());
  CHECK(node_2 == &list.pop_back());
  CHECK(node_1 == &list.pop_back());
}

TEST(List, should_pop_nodes_from_back_in_reverse_order_that_they_were_pushed_front)
{
  list.push_front(node_1);
  list.push_front(node_2);
  list.push_front(node_3);

  CHECK(node_1 == &list.pop_back());
  CHECK(node_2 == &list.pop_back());
  CHECK(node_3 == &list.pop_back());
}

TEST(List, should_pop_nodes_from_front_in_reverse_order_that_they_were_pushed_back)
{
  list.push_back(node_1);
  list.push_back(node_2);
  list.push_back(node_3);

  CHECK(node_1 == &list.pop_front());
  CHECK(node_2 == &list.pop_front());
  CHECK(node_3 == &list.pop_front());
}

TEST(List, should_remove_nodes_from_front)
{
  list.push_front(node_1);
  list.push_front(node_2);
  list.push_front(node_3);

  list.remove(node_3);

  CHECK(node_2 == &list.pop_front());
  CHECK(node_1 == &list.pop_front());
}

TEST(List, should_remove_nodes_from_back)
{
  list.push_front(node_1);
  list.push_front(node_2);
  list.push_front(node_3);

  list.remove(node_1);

  CHECK(node_3 == &list.pop_front());
  CHECK(node_2 == &list.pop_front());
}

TEST(List, should_remove_nodes_from_middle)
{
  list.push_front(node_1);
  list.push_front(node_2);
  list.push_front(node_3);

  list.remove(node_2);

  CHECK(node_3 == &list.pop_front());
  CHECK(node_1 == &list.pop_front());
}

TEST(List, should_not_fail_if_removed_node_is_not_in_the_list)
{
  list.push_front(node_1);
  list.push_front(node_2);

  list.remove(node_3);

  CHECK(node_2 == &list.pop_front());
  CHECK(node_1 == &list.pop_front());
}

TEST(List, should_iterate)
{
  list.push_back(node_1);
  list.push_back(node_2);
  list.push_back(node_3);

  auto iter = list.begin();

  CHECK(*iter == &node_1);

  ++iter;
  CHECK(*iter == &node_2);

  ++iter;
  CHECK(*iter == &node_3);

  ++iter;
  CHECK(list.end() == iter);
}

TEST(List, should_allow_the_current_node_to_be_removed_during_iteration)
{
  list.push_back(node_1);
  list.push_back(node_2);
  list.push_back(node_3);

  auto iter = list.begin();

  CHECK(*iter == &node_1);
  list.remove(node_1);
  list.push_back(node_1);

  ++iter;
  CHECK(*iter == &node_2);
}

TEST(List, should_indicate_whether_list_contains_a_given_node)
{
  CHECK(false == list.contains(node_1));
  CHECK(false == list.contains(node_2));
  CHECK(false == list.contains(node_3));

  list.push_back(node_1);
  CHECK(true == list.contains(node_1));
  CHECK(false == list.contains(node_2));
  CHECK(false == list.contains(node_3));

  list.push_back(node_2);
  CHECK(true == list.contains(node_1));
  CHECK(true == list.contains(node_2));
  CHECK(false == list.contains(node_3));

  list.push_back(node_3);
  CHECK(true == list.contains(node_1));
  CHECK(true == list.contains(node_2));
  CHECK(true == list.contains(node_3));
}

TEST(List, should_give_the_index_of_a_specified_node)
{
  list.push_back(node_1);
  list.push_back(node_2);
  list.push_back(node_3);

  CHECK(0 == list.index_of(node_1));
  CHECK(1 == list.index_of(node_2));
  CHECK(2 == list.index_of(node_3));
}
