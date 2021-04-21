/*!
 * @file
 * @brief Key value store that stores all values in RAM. Read and write access is
 * constant time because all keys are are required to be consecutive starting from 0.
 *
 * It is highly recommended to use or at least start with the macros defined in
 * tiny_ram_key_value_store_macros.h to create the configuration. It is non-trivial
 * to do this at compile-time  so that the configuration can be stored in ROM.
 *
 * @warning If data is written un-aligned it may be published unaligned.
 */

#ifndef RamKeyValueStore_h
#define RamKeyValueStore_h

#include "tiny/IKeyValueStore.h"
#include "tiny/Event.h"
#include "tiny/RamKeyValueStoreMacros.h"

namespace tiny {
  class RamKeyValueStore : public IKeyValueStore {
   public:
    typedef struct
    {
      uint16_t value_offset;
      uint8_t size;
    } KeyValuePair;

    typedef struct
    {
      const KeyValuePair* key_value_pairs;
      uint8_t key_value_pair_count;
    } Configuration;

   public:
    RamKeyValueStore(const Configuration& configuration, void* storage);
    RamKeyValueStore(const RamKeyValueStore&) = delete;
    bool operator=(const RamKeyValueStore&) = delete;
    auto read(Key key, void* value) -> void override;
    auto write(Key key, const void* value) -> void override;
    auto contains(Key key) -> bool override;
    auto size(Key key) -> uint8_t override;
    auto on_change() -> IEvent<Key, const void*>& override;

   private:
    auto offset(Key key) -> uint16_t;

   private:
    Event<Key, const void*> _on_change;
    const Configuration& configuration;
    void* storage;
  };
}

#endif
