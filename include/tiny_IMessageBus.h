/*!
 * @file
 * @brief Synchronously sends messages from a publisher to all subscribers.
 */

#ifndef tiny_IMessageBus_h
#define tiny_IMessageBus_h

#include <cstdint>
#include "tiny_IEvent.h"

namespace tiny
{
  class IMessageBus
  {
   public:
    typedef uint16_t Message;

    typedef struct
    {
      Message message;
      const void* data;
    } OnReceiveArgs;

   public:
    auto virtual send(Message message, const void* data) -> void = 0;
    auto virtual on_receive() -> IEvent<OnReceiveArgs>* = 0;
  };
}

#endif
