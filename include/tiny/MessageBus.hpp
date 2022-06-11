/*!
 * @file
 * @brief Default implementation of a message bus.
 */

#ifndef tiny_MessageBus_hpp
#define tiny_MessageBus_hpp

#include "tiny/IMessageBus.hpp"
#include "tiny/Event.hpp"

namespace tiny {
  class MessageBus final : public IMessageBus {
   public:
    MessageBus();
    MessageBus(const MessageBus& other) = delete;

    auto operator=(const MessageBus& other) -> void = delete;

    auto send(Message message, const void* data) -> void override;
    auto on_receive() -> IEvent<Message, const void*>& override;

   private:
    Event<Message, const void*> _on_receive{};
  };
}

#endif
