/*!
 * @file
 * @brief Default implementation of an event. Supports an arbitrary number of subscribers.
 */

#ifndef tiny_Event_hpp
#define tiny_Event_hpp

#include "tiny/IEvent.hpp"
#include "tiny/List.hpp"

namespace tiny {
  template <typename... Args>
  class Event final : public IEvent<Args...> {
   public:
    Event() = default;

    Event(const Event& other) = delete;

    void operator=(const Event& other) = delete;

    /*!
     * Publishes the event with the given arguments.
     */
    void publish(Args... args)
    {
      for(auto& subscriber : subscribers) {
        reinterpret_cast<EventSubscription<Args...>&>(subscriber).publish(args...);
      }
    }

    void subscribe(EventSubscription<Args...>& subscription) override
    {
      subscribers.remove(subscription);
      subscribers.push_back(subscription);
    }

    void unsubscribe(EventSubscription<Args...>& subscription) override
    {
      subscribers.remove(subscription);
    }

   private:
    List subscribers{};
  };
}

#endif
