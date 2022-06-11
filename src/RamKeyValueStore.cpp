/*!
 * @file
 * @brief
 */

#include <cstring>
#include "tiny/RamKeyValueStore.hpp"

using namespace tiny;

RamKeyValueStore::RamKeyValueStore(const Configuration& configuration, void* storage)
  : configuration{configuration}, storage{storage}
{
  Key last_key = configuration.key_value_pair_count - 1;
  uint16_t last_offset = offset(last_key);
  uint8_t last_size = size(last_key);
  memset(storage, 0, last_offset + last_size);
}

auto RamKeyValueStore::read(Key key, void* value) -> void
{
  memcpy(value, (uint8_t*)storage + offset(key), size(key));
}

auto RamKeyValueStore::write(Key key, const void* value) -> void
{
  uint8_t value_size = size(key);
  void* location = (uint8_t*)storage + offset(key);

  if(memcmp(location, value, value_size)) {
    memcpy(location, value, value_size);
    _on_change.publish(key, value);
  }
}

auto RamKeyValueStore::contains(Key key) -> bool
{
  return key < configuration.key_value_pair_count;
}

auto RamKeyValueStore::size(Key key) -> uint8_t
{
  return configuration.key_value_pairs[key].size;
}

auto RamKeyValueStore::on_change() -> IEvent<Key, const void*>&
{
  return _on_change;
}

auto RamKeyValueStore::offset(Key key) -> uint16_t
{
  return configuration.key_value_pairs[key].value_offset;
}
