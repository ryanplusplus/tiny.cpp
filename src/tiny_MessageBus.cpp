/*!
 * @file
 * @brief
 */

#include "tiny_MessageBus.h"

using namespace tiny;

MessageBus::MessageBus()
  : _on_receive()
{
}

auto MessageBus::send(Message message, const void* data) -> void
{
  OnReceiveArgs args{message, data};
  this->_on_receive.publish(&args);
}

auto MessageBus::on_receive() -> IEvent<OnReceiveArgs>*
{
  return &this->_on_receive;
}
