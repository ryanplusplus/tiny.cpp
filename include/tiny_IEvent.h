/*!
 * @file
 * @brief Abstract event.
 */

#ifndef tiny_IEvent_h
#define tiny_IEvent_h

#include "tiny_EventSubscription.h"

namespace tiny
{
  template <typename Args>
  class IEvent
  {
   public:
    auto virtual subscribe(EventSubscription<Args>* subscription) -> void = 0;
    auto virtual unsubscribe(EventSubscription<Args>* subscription) -> void = 0;
  };
}

#endif
