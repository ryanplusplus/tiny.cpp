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
  template <typename... Args>
  class Event : public IEvent<Args...> {
   public:
    Event()
      : subscribers()
    {
    }

    auto publish(Args... args) -> void
    {
      for(auto subscriber : subscribers) {
        reinterpret_cast<EventSubscription<Args...>*>(subscriber)->publish(args...);
      }
    }

    auto subscribe(EventSubscription<Args...>& subscription) -> void override
    {
      subscribers.remove(&subscription);
      subscribers.push_back(&subscription);
    }

    auto unsubscribe(EventSubscription<Args...>& subscription) -> void override
    {
      subscribers.remove(&subscription);
    }

   private:
    List subscribers;
  };
}

#endif
