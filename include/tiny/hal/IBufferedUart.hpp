/*!
 * @file
 * @brief Abstract buffered UART.
 *
 * A buffered UART sends and receives chunks of bytes instead of sending and receiving
 * byte-by-byte. This can be used to abstract over a free-running UART with DMA.
 */

#ifndef tiny_hal_IBufferedUart_hpp
#define tiny_hal_IBufferedUart_hpp

#include <cstdint>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IBufferedUart {
   public:
    virtual ~IBufferedUart(){};

    /*!
     * Sends a buffer. The buffer must remain valid until the send completes.
     */
    virtual void send(const void* buffer, uint16_t buffer_size);

    /*!
     * Event raised when a buffer is finished being sent. This is raised only
     * during a call to run().
     */
    virtual IEvent<>& on_send_complete();

    /*!
     * Event raised when bytes are received. This is raised only during a call
     * to run().
     */
    virtual IEvent<const void* /* buffer */, uint16_t /* buffer length */>& on_receive();

    /*!
     * Raises receive and send complete events as necessary.
     */
    virtual void run();
  };
}

#endif
