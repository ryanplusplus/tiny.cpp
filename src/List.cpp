/*!
 * @file
 * @brief
 */

#include <cstddef>
#include "tiny/List.hpp"

using namespace tiny;

void List::push_back(Node& node)
{
  Node* current = head.next;

  while(current->next != &head) {
    current = current->next;
  }

  current->next = &node;
  node.next = &head;
}

List::Node& List::pop_back()
{
  Node* previous = head.next;
  Node* current = head.next;

  while(current->next != &head) {
    previous = current;
    current = current->next;
  }

  previous->next = &head;
  return *current;
}

void List::remove(const Node& node)
{
  Node* previous = &head;
  Node* current = head.next;

  while(current != &head) {
    if(current == &node) {
      previous->next = current->next;
      return;
    }

    previous = current;
    current = current->next;
  }
}

uint16_t List::count() const
{
  Node* current = head.next;
  uint16_t count = 0;

  while(current != &head) {
    count++;
    current = current->next;
  }

  return count;
}

bool List::contains(const Node& node) const
{
  Node* current = head.next;

  while(current != &head) {
    if(current == &node) {
      return true;
    }

    current = current->next;
  }

  return false;
}

uint16_t List::index_of(const Node& node) const
{
  Node* current = head.next;
  uint16_t index = 0;

  while(current != &node) {
    index++;
    current = current->next;
  }

  return index;
}
