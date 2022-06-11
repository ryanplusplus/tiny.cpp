/*!
 * @file
 * @brief Linked list that contains nodes allocated by clients.
 *
 * Nodes can contain arbitrary data by defining a type that sub-classes
 * tiny::List::Node:
 *
 * struct ClientNode : public tiny::List::Node {
 *   int data;
 * }
 */

#ifndef tiny_List_h
#define tiny_List_h

#include <cstdbool>
#include <cstdint>

namespace tiny {
  class List {
   public:
    class Node {
      friend class List;

     public:
      auto operator==(const Node* other) -> bool
      {
        return this == other;
      }

     private:
      Node* next{};
    };

    class Iterator;

   public:
    List()
    {
      head.next = &head;
    }

    List(const List& other) = delete;

    auto operator=(const List& other) -> void = delete;

    auto push_front(Node& node) -> void
    {
      node.next = head.next;
      head.next = &node;
    }

    auto push_back(Node& node) -> void;

    auto pop_front() -> Node*
    {
      Node* popped = head.next;
      head.next = head.next->next;
      return popped;
    }

    auto pop_back() -> Node*;
    auto remove(const Node& node) -> void;
    auto count() -> uint16_t;
    auto contains(const Node& node) -> bool;
    auto index_of(const Node& node) -> uint16_t;

    auto begin() -> Iterator
    {
      return Iterator::begin(*this);
    }

    auto end() -> Iterator
    {
      return Iterator::end(*this);
    }

   private:
    Node head{};

   public:
    class Iterator {
     private:
      explicit Iterator(Node& current)
        : current{&current},
          next{current.next}
      {
      }

     public:
      static auto begin(List& list) -> Iterator
      {
        return Iterator(*list.head.next);
      }

      static auto end(List& list) -> Iterator
      {
        return Iterator(list.head);
      }

      bool operator==(const Iterator& other)
      {
        return current == other.current;
      }

      bool operator!=(const Iterator& other)
      {
        return !operator==(other);
      }

      Iterator& operator++()
      {
        current = next;
        next = current->next;
        return *this;
      }

      Node& operator*() const
      {
        return *current;
      }

     private:
      Node* current;
      Node* next;
    };
  };
}

#endif
