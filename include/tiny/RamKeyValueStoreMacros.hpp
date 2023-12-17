/*!
 * @file
 * @brief Helper macros for defining a RAM storage type and configuration.
 *
 * Example:
 *
 * #define my_key_value_pair_list(pair) \
 *   pair(key_foo, uint8_t) \
 *   pair(key_bar, uint32_t) \
 *
 * // Generates an enumeration that will allow your application to reference
 * // keys by name
 * enumerate_ram_key_value_pairs(my_key_value_pair_list);
 *
 * // Configures the name of the created storage struct
 * #define _storage_type_name my_storage_type_t
 *
 * // Generates the storage type with the configured name
 * // This is used to generate the configuration and is sized appropriately to
 * // hold all values
 * generate_storage_type_for_ram_key_value_pairs(my_key_value_pair_list);
 *
 * static const tiny::RamKeyValueStore::KeyValuePair my_key_value_pairs[] = {
 *   generate_configuration_pairs_from_ram_key_value_pairs(my_key_value_pair_list)
 * };
 *
 * static const tiny::RamKeyValueStore::Configuration configuration = {
 *   my_key_value_pairs,
 *   std::size(my_key_value_pairs)
 * };
 */

#ifndef tiny_RamKeyValueStoreMacros_hpp
#define tiny_RamKeyValueStoreMacros_hpp

#include <cstddef>
#include <cstdint>

#define ram_key_value_pairs_expand_as_enumeration(_key, value_type) _key,

#define enumerate_ram_key_value_pairs(_key_value_pairs)         \
  enum {                                                        \
    _key_value_pairs(ram_key_value_pairs_expand_as_enumeration) \
  }

#define ram_key_value_pairs_expand_as_storage_item(_key, _value_type) \
  uint8_t _key[sizeof(_value_type)];

#define generate_storage_type_for_ram_key_value_pairs(_key_value_pairs) \
  struct _storage_type_name {                                           \
    _key_value_pairs(ram_key_value_pairs_expand_as_storage_item)        \
  }

#define ram_key_value_pairs_expand_as_configuration_item(_key, _value_type) \
  { offsetof(_storage_type_name, _key), sizeof(_value_type) },

#define generate_configuration_pairs_from_ram_key_value_pairs(_key_value_pairs) \
  _key_value_pairs(ram_key_value_pairs_expand_as_configuration_item)

#endif
