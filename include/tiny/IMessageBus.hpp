/*!
 * @file
 * @brief Synchronously sends messages from a publisher to all subscribers.
 */

#ifndef tiny_IMessageBus_hpp
#define tiny_IMessageBus_hpp

#include <cstdint>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IMessageBus {
   public:
    typedef uint16_t Message;

   public:
    virtual ~IMessageBus(){};
    virtual void send(Message message, const void* data) = 0;
    virtual IEvent<Message, const void*>& on_receive() = 0;
  };
}

#endif
