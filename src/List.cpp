/*!
 * @file
 * @brief
 */

#include <cstddef>
#include "tiny/List.h"

using namespace tiny;

auto List::push_back(Node* node) -> void
{
  Node* current = this->head.next;

  while(current->next != &this->head) {
    current = current->next;
  }

  current->next = node;
  node->next = &this->head;
}

auto List::pop_back() -> Node*
{
  Node* previous = this->head.next;
  Node* current = this->head.next;

  while(current->next != &this->head) {
    previous = current;
    current = current->next;
  }

  previous->next = &this->head;
  return current;
}

auto List::remove(Node* node) -> void
{
  Node* previous = &this->head;
  Node* current = this->head.next;

  while(current != &this->head) {
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
  Node* current = this->head.next;
  uint16_t count = 0;

  while(current != &this->head) {
    count++;
    current = current->next;
  }

  return count;
}

auto List::contains(Node* node) -> bool
{
  Node* current = this->head.next;

  while(current != &this->head) {
    if(current == node) {
      return true;
    }

    current = current->next;
  }

  return false;
}

auto List::index_of(Node* node) -> uint16_t
{
  Node* current = this->head.next;
  uint16_t index = 0;

  while(current != node) {
    index++;
    current = current->next;
  }

  return index;
}
