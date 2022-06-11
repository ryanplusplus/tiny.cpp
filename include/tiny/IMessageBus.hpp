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
    virtual auto send(Message message, const void* data) -> void = 0;
    virtual auto on_receive() -> IEvent<Message, const void*>& = 0;
  };
}

#endif
