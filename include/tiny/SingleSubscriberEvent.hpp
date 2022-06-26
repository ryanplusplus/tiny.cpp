/*!
 * @file
 * @brief Event implementation that supports only a single subscriber. This reduces
 * stack usage and execution time when the event is published, but limits the event to
 * a single subscriber.
 */

#ifndef tiny_SingleSubscriberEvent_hpp
#define tiny_SingleSubscriberEvent_hpp

#include <cstddef>
#include "tiny/IEvent.hpp"

namespace tiny {
  template <typename... Args>
  class SingleSubscriberEvent final : public IEvent<Args...> {
   public:
    SingleSubscriberEvent() = default;

    SingleSubscriberEvent(const SingleSubscriberEvent&) = delete;
    bool operator=(const SingleSubscriberEvent&) = delete;

    void publish(Args... args) const
    {
      if(subscription) {
        subscription->publish(args...);
      }
    }

    void subscribe(EventSubscription<Args...>& subscription) override
    {
      this->subscription = &subscription;
    }

    void unsubscribe(EventSubscription<Args...>& subscription) override
    {
      if(this->subscription == &subscription) {
        this->subscription = nullptr;
      }
    }

   private:
    EventSubscription<Args...>* subscription{};
  };
}

#endif
