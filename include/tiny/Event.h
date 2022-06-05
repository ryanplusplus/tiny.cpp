/*!
 * @file
 * @brief Default implementation of an event. Supports an arbitrary number of
 * subscribers.
 */

#ifndef tiny_Event_h
#define tiny_Event_h

#include "tiny/IEvent.h"
#include "tiny/List.h"

namespace tiny {
  template <typename... Args>
  class Event final : public IEvent<Args...> {
   public:
    Event()
      : subscribers()
    {
    }

    Event(const Event& other) = delete;

    auto operator=(const Event& other) -> void = delete;

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
