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
    class ConstIterator;

   public:
    List()
    {
      head.next = &head;
    }

    List(const List& other) = delete;

    void operator=(const List& other) = delete;

    /*!
     * Adds the node to the front of the list.
     */
    void push_front(Node& node)
    {
      node.next = head.next;
      head.next = &node;
    }

    /*!
     * Adds the node to the back of the list.
     */
    void push_back(Node& node);

    /*!
     * Inserts a node after the specified node.
     */
    void insert_after(Node& after, Node& to_insert)
    {
      to_insert.next = after.next;
      after.next = &to_insert;
    }

    /*!
     * Removes the node from the front of the list. Returns the node.
     */
    Node& pop_front()
    {
      Node* popped = head.next;
      head.next = head.next->next;
      return *popped;
    }

    /*!
     * Removes the node at the back of the list. Returns the node.
     */
    Node& pop_back();

    /*!
     * Removes a specified node if present in the list.
     */
    void remove(const Node& node);

    /*!
     * Returns the number of nodes contained in the list.
     */
    uint16_t count() const;

    /*!
     * Returns true if the specified node is in the list and false otherwise.
     */
    bool contains(const Node& node) const;

    /*!
     * Gives the index of a given node in the list.
     */
    uint16_t index_of(const Node& node) const;

    Iterator begin()
    {
      return Iterator::begin(*this);
    }

    ConstIterator begin() const
    {
      return ConstIterator::begin(*this);
    }

    Iterator end()
    {
      return Iterator::end(*this);
    }

    ConstIterator end() const
    {
      return ConstIterator::end(*this);
    }

   private:
    Node head{};

   public:
    class Iterator {
     private:
      explicit Iterator(Node& current)
        : current{ &current },
          next{ current.next }
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

      bool operator==(const Iterator& other) const
      {
        return current == other.current;
      }

      bool operator!=(const Iterator& other) const
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

    class ConstIterator {
     private:
      explicit ConstIterator(const Node& current)
        : current{ &current },
          next{ current.next }
      {
      }

     public:
      static ConstIterator begin(const List& list)
      {
        return ConstIterator(*list.head.next);
      }

      static ConstIterator end(const List& list)
      {
        return ConstIterator(list.head);
      }

      bool operator==(const ConstIterator& other) const
      {
        return current == other.current;
      }

      bool operator!=(const ConstIterator& other)
      {
        return !operator==(other);
      }

      ConstIterator& operator++()
      {
        current = next;
        next = current->next;
        return *this;
      }

      const Node& operator*() const
      {
        return *current;
      }

     private:
      const Node* current;
      const Node* next;
    };
  };
}

#endif
