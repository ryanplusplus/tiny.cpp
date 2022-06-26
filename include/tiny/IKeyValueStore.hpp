/*!
 * @file
 * @brief Key value store using pre-defined keys and fixed-sized values.
 */

#ifndef tiny_IKeyValueStore_hpp
#define tiny_IKeyValueStore_hpp

#include <cstdint>
#include "tiny/IEvent.hpp"

namespace tiny {
  class IKeyValueStore {
   public:
    typedef uint8_t Key;

   public:
    virtual ~IKeyValueStore(){};
    virtual void read(Key key, void* value) = 0;
    virtual void write(Key key, const void* value) = 0;
    virtual bool contains(Key key) = 0;
    virtual uint8_t size(Key key) = 0;
    virtual IEvent<Key, const void*>& on_change() = 0;
  };
}

#endif
