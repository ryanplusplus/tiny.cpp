/*!
 * @file
 * @brief
 */

#include <cstring>
#include "tiny/RamKeyValueStore.h"

using namespace tiny;

RamKeyValueStore::RamKeyValueStore(const Configuration& configuration, void* storage)
  : _on_change(), configuration(configuration), storage(storage)
{
  Key last_key = this->configuration.key_value_pair_count - 1;
  uint16_t last_offset = this->offset(last_key);
  uint8_t last_size = this->size(last_key);
  memset(this->storage, 0, last_offset + last_size);
}

auto RamKeyValueStore::read(Key key, void* value) -> void
{
  memcpy(value, (uint8_t*)this->storage + this->offset(key), this->size(key));
}

auto RamKeyValueStore::write(Key key, const void* value) -> void
{
  uint8_t value_size = this->size(key);
  void* location = (uint8_t*)this->storage + this->offset(key);

  if(memcmp(location, value, value_size)) {
    memcpy(location, value, value_size);
    this->_on_change.publish(key, value);
  }
}

auto RamKeyValueStore::contains(Key key) -> bool
{
  return key < this->configuration.key_value_pair_count;
}

auto RamKeyValueStore::size(Key key) -> uint8_t
{
  return this->configuration.key_value_pairs[key].size;
}

auto RamKeyValueStore::on_change() -> IEvent<Key, const void*>&
{
  return this->_on_change;
}

auto RamKeyValueStore::offset(Key key) -> uint16_t
{
  return this->configuration.key_value_pairs[key].value_offset;
}
