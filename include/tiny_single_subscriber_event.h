/*!
 * @file
 * @brief Event implementation that supports only a single subscriber. This reduces
 * stack usage and execution time when the event is published, but limits the event to
 * a single subscriber.
 */

#ifndef tiny_single_subscriber_event_h
#define tiny_single_subscriber_event_h

#include <cstddef>
#include "i_tiny_event.h"

namespace tiny
{
  template <typename Args>
  class SingleSubscriberEvent : public I_Event<Args>
  {
   public:
    SingleSubscriberEvent()
      : subscription()
    {
    }

    auto publish(const Args* args)
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
        this->subscription = NULL;
      }
    }

   private:
    EventSubscription<Args>* subscription;
  };
}

#endif
