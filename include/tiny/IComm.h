/*!
 * @file
 * @brief Abstraction over a point-to-point communication link.
 */

#ifndef tiny_IComm_h
#define tiny_IComm_h

#include <cstdint>
#include <cstdbool>
#include "tiny/IEvent.h"

namespace tiny {
  class IComm {
   public:
    virtual ~IComm(){};
    virtual auto send(const void* payload, uint8_t length) -> void = 0;
    virtual auto sending() -> bool = 0;
    virtual auto on_receive() -> IEvent<const void*, uint8_t>& = 0;
  };
}

#endif
