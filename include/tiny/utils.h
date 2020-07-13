/*!
 * @file
 * @brief
 */

#ifndef utils_h
#define utils_h

#define element_count(_array) \
  (sizeof(_array) / sizeof(_array[0]))

#define element_size(_array) \
  sizeof(_array[0])

#define container_of(_container_type, _field_name, _field) \
  (_container_type*)((uint8_t*)_field - offsetof(_container_type, _field_name))

#endif
