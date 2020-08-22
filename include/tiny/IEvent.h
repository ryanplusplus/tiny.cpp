/*!
 * @file
 * @brief Abstract event.
 */

#ifndef IEvent_h
#define IEvent_h

#include "tiny/EventSubscription.h"

namespace tiny {
  template <typename Args>
  class IEvent {
   public:
    virtual ~IEvent(){};
    auto virtual subscribe(EventSubscription<Args>* subscription) -> void = 0;
    auto virtual unsubscribe(EventSubscription<Args>* subscription) -> void = 0;
  };
}

#endif
