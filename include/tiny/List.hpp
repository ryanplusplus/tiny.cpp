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

    template <typename List, typename Node>
    class IteratorImpl;
    using Iterator = IteratorImpl<List, Node>;
    using ConstIterator = IteratorImpl<const List, const Node>;

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
    template <typename List, typename Node>
    class IteratorImpl {
     private:
      explicit IteratorImpl(Node& current)
        : current{ &current },
          next{ current.next }
      {
      }

     public:
      static IteratorImpl<List, Node> begin(List& list)
      {
        return IteratorImpl<List, Node>(*list.head.next);
      }

      static IteratorImpl<List, Node> end(List& list)
      {
        return IteratorImpl<List, Node>(list.head);
      }

      bool operator==(const IteratorImpl<List, Node>& other) const
      {
        return current == other.current;
      }

      bool operator!=(const IteratorImpl<List, Node>& other) const
      {
        return !operator==(other);
      }

      IteratorImpl<List, Node>& operator++()
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
