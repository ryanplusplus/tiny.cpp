/*!
 * @file
 * @brief Abstract event queue.
 *
 * Allows event callbacks and data to be queued for later processing.
 */

#ifndef tiny_IEventQueue_hpp
#define tiny_IEventQueue_hpp

#include <cstdint>

namespace tiny {
  class IEventQueue {
   public:
    typedef void (*Callback)(void);
    typedef void (*CallbackWithData)(const void* data);

   public:
    virtual ~IEventQueue(){};

    /*!
     * Enqueues an event to be raised later by invoking the provided callback.
     */
    virtual void enqueue(Callback callback) = 0;

    /*!
     * Enqueues an event with data to be raised later by invoking the provided callback.
     *
     * @note data is copied and does not have to remain valid after the call
     */
    virtual void enqueue(CallbackWithData callback, const void* data, uint8_t dataSize) = 0;
  };
}

#endif
