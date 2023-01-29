/*!
 * @file
 * @brief Abstract key value store using pre-defined keys and fixed-sized values.
 *
 * Provides notifications when stored values change.
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

    /*!
     * Reads the value corresponding to the key into the provided buffer. Note that ensuring
     * that the buffer is large enough to store the value is the responsibility of the client.
     */
    virtual void read(Key key, void* value) const = 0;

    /*!
     * Reads a key by value.
     */
    template <typename T>
    T read_value(Key key) const
    {
      T value;
      read(key, &value);
      return value;
    }

    /*!
     * Writes the value from the provided buffer to the storage associated with the provided
     * key. Note that ensuring that the provided buffer is sized appropriately is the
     * responsibility of the client.
     */
    virtual void write(Key key, const void* value) = 0;

    /*!
     * Writes a key by value.
     */
    template <typename T>
    void write_value(Key key, T value)
    {
      write(key, &value);
    }

    /*!
     * Returns true if the key value store contains data corresponding to the provided key.
     */
    virtual bool contains(Key key) const = 0;

    /*!
     * Returns the size in bytes of the value associated with the provided key.
     */
    virtual uint8_t size(Key key) const = 0;

    /*!
     * Provides access to an event raised each time the value for a key-value pair changes.
     */
    virtual IEvent<Key, const void*>& on_change() = 0;
  };
}

#endif
