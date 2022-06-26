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

#ifndef tiny_List_hpp
#define tiny_List_hpp

#include <cstdbool>
#include <cstdint>

namespace tiny {
  class List {
   public:
    class Node {
      friend class List;

     public:
      bool operator==(const Node* other)
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

    void operator=(const List& other) = delete;

    void push_front(Node& node)
    {
      node.next = head.next;
      head.next = &node;
    }

    void push_back(Node& node);

    void insert_after(Node& after, Node& to_insert)
    {
      to_insert.next = after.next;
      after.next = &to_insert;
    }

    Node& pop_front()
    {
      Node* popped = head.next;
      head.next = head.next->next;
      return *popped;
    }

    Node& pop_back();
    void remove(const Node& node);
    uint16_t count();
    bool contains(const Node& node);
    uint16_t index_of(const Node& node);

    Iterator begin()
    {
      return Iterator::begin(*this);
    }

    Iterator end()
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
      static Iterator begin(List& list)
      {
        return Iterator(*list.head.next);
      }

      static Iterator end(List& list)
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
