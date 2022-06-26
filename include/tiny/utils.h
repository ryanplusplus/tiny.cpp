/*!
 * @file
 * @brief
 */

#ifndef tiny_utils_h
#define tiny_utils_h

/*!
 * Calculates the number of elements in an array.
 */
#define element_count(_array) \
  (sizeof(_array) / sizeof(_array[0]))

/*!
 * Calculates the size of the elements in an array.
 */
#define element_size(_array) \
  sizeof(_array[0])

#endif
