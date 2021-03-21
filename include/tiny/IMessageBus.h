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

    typedef struct
    {
      Message message;
      const void* data;
    } OnReceiveArgs;

   public:
    virtual ~IMessageBus(){};
    auto virtual send(Message message, const void* data) -> void = 0;
    auto virtual on_receive() -> IEvent<OnReceiveArgs>& = 0;
  };
}

#endif
