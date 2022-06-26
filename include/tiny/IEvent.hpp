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

    /*!
     * Adds a subscriber to the event.
     */
    virtual void subscribe(EventSubscription<Args...>& subscription) = 0;

    /*!
     * Removes a subscriber from the event.
     */
    virtual void unsubscribe(EventSubscription<Args...>& subscription) = 0;
  };
}

#endif
