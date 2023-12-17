/*!
 * @file
 * @brief Key value store that stores all values in RAM. Read and write access is
 * constant time because all keys are are required to be consecutive starting from 0.
 *
 * It is highly recommended to use or at least start with the macros defined in
 * RamKeyValueStoreMacros.hpp to create the configuration. It is non-trivial
 * to do this at compile-time so that the configuration can be stored in ROM.
 *
 * @warning If data is written unaligned it may be published unaligned.
 */

#ifndef tiny_RamKeyValueStore_hpp
#define tiny_RamKeyValueStore_hpp

#include "tiny/Event.hpp"
#include "tiny/IKeyValueStore.hpp"
#include "tiny/RamKeyValueStoreMacros.hpp"

namespace tiny {
  class RamKeyValueStore final : public IKeyValueStore {
   public:
    struct KeyValuePair {
      uint16_t value_offset;
      uint8_t size;
    };

    struct Configuration {
      const KeyValuePair* key_value_pairs;
      uint8_t key_value_pair_count;
    };

   public:
    /*!
     * Initializes a key value store. All values are initialized to 0.
     */
    RamKeyValueStore(const Configuration& configuration, void* storage);

    RamKeyValueStore(const RamKeyValueStore&) = delete;

    bool operator=(const RamKeyValueStore&) = delete;

    void read(Key key, void* value) const override;
    void write(Key key, const void* value) override;
    bool contains(Key key) const override;
    uint8_t size(Key key) const override;
    IEvent<Key, const void*>& on_change() override;

   private:
    uint16_t offset(Key key) const;

   private:
    Event<Key, const void*> _on_change{};
    const Configuration& configuration;
    void* storage;
  };
}

#endif
