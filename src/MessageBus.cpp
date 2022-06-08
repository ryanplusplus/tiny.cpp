/*!
 * @file
 * @brief
 */

#include "tiny/MessageBus.h"

using namespace tiny;

MessageBus::MessageBus()
{
}

auto MessageBus::send(Message message, const void* data) -> void
{
  _on_receive.publish(message, data);
}

auto MessageBus::on_receive() -> IEvent<Message, const void*>&
{
  return _on_receive;
}
