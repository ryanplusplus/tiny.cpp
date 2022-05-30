/*!
 * @file
 * @brief Abstract event queue.
 *
 * Allows event callbacks and data to be queued for later processing.
 */

#ifndef tiny_IEventQueue_h
#define tiny_IEventQueue_h

#include <cstdint>

namespace tiny {
  class IEventQueue {
   public:
    typedef void (*Callback)(void);
    typedef void (*CallbackWithData)(const void* data);

   public:
    virtual ~IEventQueue(){};
    virtual auto enqueue(Callback callback) -> void = 0;
    virtual auto enqueue(CallbackWithData callback, const void* data, uint8_t dataSize) -> void = 0;
  };
}

#endif
