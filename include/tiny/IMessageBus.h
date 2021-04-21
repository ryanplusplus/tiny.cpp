/*!
 * @file
 * @brief Synchronously sends messages from a publisher to all subscribers.
 */

#ifndef IMessageBus_h
#define IMessageBus_h

#include <cstdint>
#include "tiny/IEvent.h"

namespace tiny {
  class IMessageBus {
   public:
    typedef uint16_t Message;

   public:
    virtual ~IMessageBus(){};
    virtual auto send(Message message, const void* data) -> void = 0;
    virtual auto on_receive() -> IEvent<Message, const void*>& = 0;
  };
}

#endif
