/*!
 * @file
 * @brief
 */

#include <string.h>
#include "tiny/RamKeyValueStore.h"
#include "tiny/utils.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;

// clang-format off
#define my_key_value_pair_list(pair) \
  pair(key_foo, uint8_t)             \
  pair(key_bar, uint32_t)
// clang-format on

enumerate_ram_key_value_pairs(my_key_value_pair_list);

#define _storage_type_name Storage

generate_storage_type_for_ram_key_value_pairs(my_key_value_pair_list);

static const RamKeyValueStore::KeyValuePair my_key_value_pairs[] = {
  generate_configuration_pairs_from_ram_key_value_pairs(my_key_value_pair_list)};

static const RamKeyValueStore::Configuration configuration = {
  my_key_value_pairs,
  element_count(my_key_value_pairs),
};

TEST_GROUP(ram_key_value_store)
{
  Storage storage{0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
  RamKeyValueStore kvs{&configuration, &storage};
  EventSubscription<RamKeyValueStore::OnChangeArgs> on_change_subscription{(void*)nullptr, value_changed};

  static void value_changed(void* context, const RamKeyValueStore::OnChangeArgs* args)
  {
    (void)context;
    mock()
      .actualCall("value_changed")
      .withParameter("key", args->key)
      .withParameter("value", *(const uint32_t*)args->value);
  }

  void should_contain_key(IKeyValueStore::Key key)
  {
    CHECK_TRUE(kvs.contains(key));
  }

  void should_not_contain_key(IKeyValueStore::Key key)
  {
    CHECK_FALSE(kvs.contains(key));
  }

  void value_for_key_should_have_size(IKeyValueStore::Key key, uint8_t size)
  {
    CHECK_EQUAL(size, kvs.size(key));
  }

  void key_should_have_value(IKeyValueStore::Key key, uint8_t expected)
  {
    uint8_t actual;
    kvs.read(key, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void key_should_have_value(IKeyValueStore::Key key, uint32_t expected)
  {
    uint32_t actual;
    kvs.read(key, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void after_key_is_written_with(IKeyValueStore::Key key, uint8_t value)
  {
    kvs.write(key, &value);
  }

  void after_key_is_written_with(IKeyValueStore::Key key, uint32_t value)
  {
    kvs.write(key, &value);
  }

  void given_that_an_on_change_subscription_is_active()
  {
    kvs.on_change()->subscribe(&on_change_subscription);
  }

  void an_on_change_publication_should_be_received(IKeyValueStore::Key key, uint32_t value)
  {
    mock()
      .expectOneCall("value_changed")
      .withParameter("key", key)
      .withParameter("value", value);
  }
};

TEST(ram_key_value_store, should_contain_only_configured_keys)
{
  should_contain_key(key_foo);
  should_contain_key(key_bar);
  should_not_contain_key(2);
  should_not_contain_key(0xFF);
}

TEST(ram_key_value_store, should_give_the_size_of_contained_values)
{
  value_for_key_should_have_size(key_foo, sizeof(uint8_t));
  value_for_key_should_have_size(key_bar, sizeof(uint32_t));
}

TEST(ram_key_value_store, should_initialize_all_values_to_zero)
{
  key_should_have_value(key_foo, (uint8_t)0);
  key_should_have_value(key_bar, (uint32_t)0);
}

TEST(ram_key_value_store, should_allow_items_to_be_written_and_read)
{
  after_key_is_written_with(key_foo, (uint8_t)0xAB);
  key_should_have_value(key_foo, (uint8_t)0xAB);

  after_key_is_written_with(key_bar, (uint32_t)0x12345678);
  key_should_have_value(key_bar, (uint32_t)0x12345678);
}

TEST(ram_key_value_store, should_raise_on_change_event_when_a_value_changes)
{
  given_that_an_on_change_subscription_is_active();
  an_on_change_publication_should_be_received(key_bar, 0x12345678);
  after_key_is_written_with(key_bar, (uint32_t)0x12345678);
}
