/*!
 * @file
 * @brief Key value store using pre-defined keys and fixed-sized values.
 */

#ifndef IKeyValueStore_h
#define IKeyValueStore_h

#include <cstdint>
#include "tiny/IEvent.h"

namespace tiny
{
  class IKeyValueStore
  {
   public:
    typedef uint8_t Key;

    typedef struct
    {
      Key key;
      const void* value;
    } OnChangeArgs;

   public:
    auto virtual read(Key key, void* value) -> void = 0;
    auto virtual write(Key key, const void* value) -> void = 0;
    auto virtual contains(Key key) -> bool = 0;
    auto virtual size(Key key) -> uint8_t = 0;
    auto virtual on_change() -> IEvent<OnChangeArgs>* = 0;
  };
}

#endif
