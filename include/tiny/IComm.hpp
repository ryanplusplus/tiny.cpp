/*!
 * @file
 * @brief Abstraction over a point-to-point communication link.
 */

#ifndef tiny_IComm_hpp
#define tiny_IComm_hpp

#include <cstdint>
#include <cstdbool>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IComm {
   public:
    virtual ~IComm(){};
    virtual void send(const void* payload, uint8_t length) = 0;
    virtual bool sending() = 0;
    virtual IEvent<const void*, uint8_t>& on_receive() = 0;
  };
}

#endif
