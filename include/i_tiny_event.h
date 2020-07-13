/*!
 * @file
 * @brief Abstract event.
 */

#ifndef i_tiny_event_h
#define i_tiny_event_h

#include "tiny_event_subscription.h"

namespace tiny
{
  template <typename Args>
  class I_Event
  {
   public:
    auto virtual subscribe(EventSubscription<Args>* subscription) -> void = 0;
    auto virtual unsubscribe(EventSubscription<Args>* subscription) -> void = 0;
  };
}

#endif
