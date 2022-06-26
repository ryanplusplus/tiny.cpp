/*!
 * @file
 * @brief
 */

#include "tiny/MessageBus.hpp"

using namespace tiny;

MessageBus::MessageBus()
{
}

void MessageBus::send(Message message, const void* data)
{
  _on_receive.publish(message, data);
}

IEvent<MessageBus::Message, const void*>& MessageBus::on_receive()
{
  return _on_receive;
}
