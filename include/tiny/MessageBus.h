/*!
 * @file
 * @brief Default implementation of a message bus.
 */

#ifndef MessageBus_h
#define MessageBus_h

#include "tiny/IMessageBus.h"
#include "tiny/Event.h"

namespace tiny {
  class MessageBus : public IMessageBus {
   public:
    MessageBus();
    auto send(Message message, const void* data) -> void override;
    auto on_receive() -> IEvent<Message, const void*>& override;

   private:
    Event<Message, const void*> _on_receive;
  };
}

#endif
