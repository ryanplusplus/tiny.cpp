/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IUart_hpp
#define tiny_hal_IUart_hpp

#include <cstdint>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IUart {
   public:
    virtual ~IUart(){};
    virtual void send(uint8_t byte) = 0;
    virtual IEvent<>& on_send_complete() = 0;
    virtual IEvent<uint8_t>& on_receive() = 0;
  };
}

#endif
