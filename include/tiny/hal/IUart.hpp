/*!
 * @file
 * @brief Abstract UART peripheral.
 */

#ifndef tiny_hal_IUart_hpp
#define tiny_hal_IUart_hpp

#include <cstdint>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IUart {
   public:
    virtual ~IUart(){};

    /*!
     * Sends a byte.
     */
    virtual void send(uint8_t byte) = 0;

    /*!
     * Event raised when a byte is finished being sent. Clients must assume
     * that this is raised from an interrupt.
     */
    virtual IEvent<>& on_send_complete() = 0;

    /*!
     * Event raised when a byte is received. Clients must assume that this is
     * raised from an interrupt.
     */
    virtual IEvent<uint8_t>& on_receive() = 0;
  };
}

#endif
