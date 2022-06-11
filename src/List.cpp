/*!
 * @file
 * @brief
 */

#include <cstddef>
#include "tiny/List.h"

using namespace tiny;

auto List::push_back(Node* node) -> void
{
  Node* current = head.next;

  while(current->next != &head) {
    current = current->next;
  }

  current->next = node;
  node->next = &head;
}

auto List::pop_back() -> Node*
{
  Node* previous = head.next;
  Node* current = head.next;

  while(current->next != &head) {
    previous = current;
    current = current->next;
  }

  previous->next = &head;
  return current;
}

auto List::remove(const Node* node) -> void
{
  Node* previous = &head;
  Node* current = head.next;

  while(current != &head) {
    if(current == node) {
      previous->next = current->next;
      return;
    }

    previous = current;
    current = current->next;
  }
}

auto List::count() -> uint16_t
{
  Node* current = head.next;
  uint16_t count = 0;

  while(current != &head) {
    count++;
    current = current->next;
  }

  return count;
}

auto List::contains(const Node* node) -> bool
{
  Node* current = head.next;

  while(current != &head) {
    if(current == node) {
      return true;
    }

    current = current->next;
  }

  return false;
}

auto List::index_of(const Node* node) -> uint16_t
{
  Node* current = head.next;
  uint16_t index = 0;

  while(current != node) {
    index++;
    current = current->next;
  }

  return index;
}
