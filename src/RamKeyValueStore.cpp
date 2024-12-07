/*!
 * @file
 * @brief
 */

#include <cstring>
#include "tiny/RamKeyValueStore.hpp"

using namespace tiny;

RamKeyValueStore::RamKeyValueStore(const Configuration& configuration, void* storage)
  : configuration{ configuration }, storage{ storage }
{
  auto last_key = static_cast<Key>(configuration.key_value_pair_count - 1);
  uint16_t last_offset = offset(last_key);
  uint8_t last_size = size(last_key);
  memset(storage, 0, last_offset + last_size);
}

void RamKeyValueStore::read(Key key, void* value) const
{
  memcpy(value, static_cast<uint8_t*>(storage) + offset(key), size(key));
}

void RamKeyValueStore::write(Key key, const void* value)
{
  uint8_t value_size = size(key);
  void* location = static_cast<uint8_t*>(storage) + offset(key);

  if(memcmp(location, value, value_size)) {
    memcpy(location, value, value_size);
    _on_change.publish(key, value);
  }
}

bool RamKeyValueStore::contains(Key key) const
{
  return key < configuration.key_value_pair_count;
}

uint8_t RamKeyValueStore::size(Key key) const
{
  return configuration.key_value_pairs[key].size;
}

IEvent<IKeyValueStore::Key, const void*>& RamKeyValueStore::on_change()
{
  return _on_change;
}

uint16_t RamKeyValueStore::offset(Key key) const
{
  return configuration.key_value_pairs[key].value_offset;
}
