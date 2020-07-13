/*!
 * @file
 * @brief
 */

#ifndef IUart_h
#define IUart_h

#include <cstdint>
#include "tiny/IEvent.h"

namespace tiny
{
  class IUart
  {
   public:
    auto virtual send(uint8_t byte) -> void = 0;
    auto virtual on_send_complete() -> IEvent<void> = 0;
    auto virtual on_receive() -> IEvent<uint8_t> = 0;
  };
}

#endif
