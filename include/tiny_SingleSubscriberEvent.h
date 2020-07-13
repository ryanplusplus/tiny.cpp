/*!
 * @file
 * @brief Event implementation that supports only a single subscriber. This reduces
 * stack usage and execution time when the event is published, but limits the event to
 * a single subscriber.
 */

#ifndef tiny_SingleSubscriberEvent_h
#define tiny_SingleSubscriberEvent_h

#include <cstddef>
#include "tiny_IEvent.h"

namespace tiny
{
  template <typename Args>
  class SingleSubscriberEvent : public IEvent<Args>
  {
   public:
    SingleSubscriberEvent()
      : subscription()
    {
    }

    auto publish(const Args* args) const -> void
    {
      if(this->subscription)
      {
        this->subscription->publish(args);
      }
    }

    auto subscribe(EventSubscription<Args>* subscription) -> void
    {
      this->subscription = subscription;
    }

    auto unsubscribe(EventSubscription<Args>* subscription) -> void
    {
      if(this->subscription == subscription)
      {
        this->subscription = nullptr;
      }
    }

   private:
    EventSubscription<Args>* subscription;
  };
}

#endif
