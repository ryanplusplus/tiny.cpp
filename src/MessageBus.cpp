/*!
 * @file
 * @brief
 */

#include "tiny/MessageBus.h"

using namespace tiny;

MessageBus::MessageBus()
  : _on_receive()
{
}

auto MessageBus::send(Message message, const void* data) -> void
{
  this->_on_receive.publish(message, data);
}

auto MessageBus::on_receive() -> IEvent<Message, const void*>&
{
  return this->_on_receive;
}
