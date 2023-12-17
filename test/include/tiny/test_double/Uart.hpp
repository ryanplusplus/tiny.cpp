/*!
 * @file
 * @brief UART test double.
 */

#ifndef tiny_test_double_Uart_hpp
#define tiny_test_double_Uart_hpp

#include "CppUTestExt/MockSupport.h"
#include "tiny/Event.hpp"
#include "tiny/hal/IUart.hpp"

namespace tiny::test_double {
  class Uart final : public IUart {
   public:
    Uart()
      : receive{}, send_complete{}, _sending{ false }
    {
    }

    Uart(const Uart& other) = delete;

    void operator=(const Uart& other) = delete;

    /*!
     * True if a byte was sent without send completing.
     */
    bool sending() const
    {
      return _sending;
    };

    /*!
     * Causes the double to raise a send complete event.
     */
    void trigger_send_complete()
    {
      _sending = false;
      send_complete.publish();
    }

    /*!
     * Causes the double to raise a receive event.
     */
    void trigger_receive(uint8_t byte)
    {
      receive.publish(byte);
    }

    void send(uint8_t byte) override
    {
      _sending = true;
      mock().actualCall("send").onObject(this).withParameter("byte", byte);
    }

    virtual IEvent<>& on_send_complete() override
    {
      return send_complete;
    }

    virtual IEvent<uint8_t>& on_receive() override
    {
      return receive;
    }

   private:
    Event<uint8_t> receive;
    Event<> send_complete;
    bool _sending;
  };
}

#endif
