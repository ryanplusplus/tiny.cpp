/*!
 * @file
 * @brief Default implementation of a message bus.
 */

#ifndef tiny_MessageBus_hpp
#define tiny_MessageBus_hpp

#include "tiny/Event.hpp"
#include "tiny/IMessageBus.hpp"

namespace tiny {
  class MessageBus final : public IMessageBus {
   public:
    MessageBus();
    MessageBus(const MessageBus& other) = delete;

    void operator=(const MessageBus& other) = delete;

    void send(Message message, const void* data) override;
    IEvent<Message, const void*>& on_receive() override;

   private:
    Event<Message, const void*> _on_receive{};
  };
}

#endif
