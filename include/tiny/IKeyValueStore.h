/*!
 * @file
 * @brief Key value store using pre-defined keys and fixed-sized values.
 */

#ifndef IKeyValueStore_h
#define IKeyValueStore_h

#include <cstdint>
#include "tiny/IEvent.h"

namespace tiny {
  class IKeyValueStore {
   public:
    typedef uint8_t Key;

   public:
    virtual ~IKeyValueStore(){};
    virtual auto read(Key key, void* value) -> void = 0;
    virtual auto write(Key key, const void* value) -> void = 0;
    virtual auto contains(Key key) -> bool = 0;
    virtual auto size(Key key) -> uint8_t = 0;
    virtual auto on_change() -> IEvent<Key, const void*>& = 0;
  };
}

#endif
