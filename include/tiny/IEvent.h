/*!
 * @file
 * @brief Abstract event.
 */

#ifndef IEvent_h
#define IEvent_h

#include "tiny/EventSubscription.h"

namespace tiny {
  template <typename... Args>
  class IEvent {
   public:
    virtual ~IEvent(){};
    virtual auto subscribe(EventSubscription<Args...>& subscription) -> void = 0;
    virtual auto unsubscribe(EventSubscription<Args...>& subscription) -> void = 0;
  };
}

#endif
