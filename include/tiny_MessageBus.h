/*!
 * @file
 * @brief Default implementation of a message bus.
 */

#ifndef tiny_MessageBus_h
#define tiny_MessageBus_h

#include "tiny_IMessageBus.h"
#include "tiny_Event.h"

namespace tiny
{
  class MessageBus : public IMessageBus
  {
   public:
    MessageBus();
    auto send(Message message, const void* data) -> void;
    auto on_receive() -> IEvent<OnReceiveArgs>*;

   private:
    Event<OnReceiveArgs> _on_receive;
  };
}

#endif
