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
    virtual auto send(uint8_t byte) -> void = 0;
    virtual auto on_send_complete() -> IEvent<>& = 0;
    virtual auto on_receive() -> IEvent<uint8_t>& = 0;
  };
}

#endif
