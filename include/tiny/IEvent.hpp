/*!
 * @file
 * @brief Abstract event.
 */

#ifndef tiny_IEvent_hpp
#define tiny_IEvent_hpp

#include "tiny/EventSubscription.hpp"

namespace tiny {
  template <typename... Args>
  class IEvent {
   public:
    virtual ~IEvent(){};
    virtual void subscribe(EventSubscription<Args...>& subscription) = 0;
    virtual void unsubscribe(EventSubscription<Args...>& subscription) = 0;
  };
}

#endif
