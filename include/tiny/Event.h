/*!
 * @file
 * @brief Default implementation of an event. Supports an arbitrary number of
 * subscribers.
 */

#ifndef Event_h
#define Event_h

#include "tiny/IEvent.h"
#include "tiny/List.h"

namespace tiny {
  template <typename Args>
  class Event : public IEvent<Args> {
   public:
    Event()
      : subscribers()
    {
    }

    auto publish(const Args* args) -> void
    {
      for(auto i = this->subscribers.begin(); i != this->subscribers.end(); ++i) {
        reinterpret_cast<EventSubscription<Args>*>(*i)->publish(args);
      }
    }

    auto subscribe(EventSubscription<Args>* subscription) -> void
    {
      this->subscribers.remove(reinterpret_cast<List::Node*>(subscription));
      this->subscribers.push_back(reinterpret_cast<List::Node*>(subscription));
    }

    auto unsubscribe(EventSubscription<Args>* subscription) -> void
    {
      this->subscribers.remove(reinterpret_cast<List::Node*>(subscription));
    }

   private:
    List subscribers;
  };
}

#endif
