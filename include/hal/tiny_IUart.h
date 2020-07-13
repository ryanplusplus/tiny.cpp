/*!
 * @file
 * @brief
 */

#ifndef tiny_IUart_h
#define tiny_IUart_h

#include <cstdint>
#include "tiny_IEvent.h"

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
