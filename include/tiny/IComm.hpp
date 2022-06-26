/*!
 * @file
 * @brief Abstract packet-based communication interface.
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

    /*!
     * Sends a message. Only valid when sending() is false.
     */
    virtual void send(const void* payload, uint8_t length) = 0;

    /*!
     * Indicates whether a message is currently being sent.
     */
    virtual bool sending() = 0;

    /*!
     * Event raised when a message is received.
     */
    virtual IEvent<const void*, uint8_t>& on_receive() = 0;
  };
}

#endif
