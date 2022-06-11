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

    auto publish(Args... args) const -> void
    {
      if(subscription) {
        subscription->publish(args...);
      }
    }

    auto subscribe(EventSubscription<Args...>& subscription) -> void override
    {
      this->subscription = &subscription;
    }

    auto unsubscribe(EventSubscription<Args...>& subscription) -> void override
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
