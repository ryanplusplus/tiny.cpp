/*!
 * @file
 * @brief Linked list that contains nodes allocated by clients.
 *
 * Nodes can contain arbitrary data by defining a type that contains
 * a tiny::List::Node:
 *
 * typedef struct client_node_t {
 *   tiny::List::Node;
 *   int data;
 * }
 *
 * This type must be cast to a tiny::List::Node to be added but can
 * be cast back by the client so that the data can be accessed.
 */

#ifndef tiny_list_h
#define tiny_list_h

#include <cstdbool>
#include <cstdint>

namespace tiny
{
  class List
  {
   public:
    class Node
    {
      friend class List;

     public:
      bool operator==(const Node* other)
      {
        return this == other;
      }

     private:
      Node* next;
    };

    class Iterator;

   public:
    List();
    auto push_front(Node* node) -> void;
    auto push_back(Node* node) -> void;
    auto pop_front() -> Node*;
    auto pop_back() -> Node*;
    auto remove(Node* node) -> void;
    auto count() -> uint16_t;
    auto contains(Node* node) -> bool;
    auto index_of(Node* node) -> uint16_t;

    auto begin() const -> Iterator
    {
      return Iterator::begin(*this);
    }

    auto end() const -> Iterator
    {
      return Iterator::end(*this);
    }

   private:
    Node head;

   public:
    class Iterator
    {
     public:
      Iterator(const Node* current)
        : current(current)
      {
      }

      static auto begin(const List& list) -> Iterator
      {
        return Iterator(list.head.next);
      }

      static auto end(const List& list) -> Iterator
      {
        return Iterator(&list.head);
      }

      bool operator==(const Iterator& other) const
      {
        return this->current == other.current;
      }

      bool operator!=(const Iterator& other) const
      {
        return !operator==(other);
      }

      void operator++()
      {
        this->current = this->current->next;
      }

      const Node* operator*() const
      {
        return this->current;
      }

     private:
      const Node* current;
    };
  };
}

#endif
