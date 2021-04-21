/*!
 * @file
 * @brief
 */

#ifndef UartDouble_h
#define UartDouble_h

#include "tiny/hal/IUart.h"
#include "tiny/Event.h"
#include "CppUTestExt/MockSupport.h"

namespace tiny {
  class UartDouble : public IUart {
   public:
    UartDouble()
      : receive(), send_complete(), _sending(false)
    {
    }

    auto sending() -> bool
    {
      return _sending;
    };

    auto trigger_send_complete() -> void
    {
      _sending = false;
      send_complete.publish();
    }

    auto trigger_receive(uint8_t byte) -> void
    {
      receive.publish(byte);
    }

    auto send(uint8_t byte) -> void override
    {
      _sending = true;
      mock().actualCall("send").onObject(this).withParameter("byte", byte);
    }

    virtual auto on_send_complete() -> IEvent<>& override
    {
      return send_complete;
    }

    virtual auto on_receive() -> IEvent<uint8_t>&
    {
      return receive;
    }

   private:
    Event<uint8_t> receive;
    Event<> send_complete;
    bool _sending;
  };
};

#endif
