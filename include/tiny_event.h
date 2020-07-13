/*!
 * @file
 * @brief Default implementation of an event. Supports an arbitrary number of
 * subscribers.
 */

#ifndef tiny_event_h
#define tiny_event_h

#include "i_tiny_event.h"
#include "tiny_list.h"

namespace tiny
{
  template <typename Args>
  class Event : public I_Event<Args>
  {
   public:
    Event()
      : subscribers()
    {
    }

    auto publish(const Args* args) -> void
    {
      for(auto i = this->subscribers.begin(); i != this->subscribers.end(); ++i)
      {
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
