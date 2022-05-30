/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogInputGroup_h
#define tiny_hal_IAnalogInputGroup_h

#include <cstdint>

namespace tiny {
  class IAnalogInputGroup {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogInputGroup(){};
    virtual auto read(Channel channel) -> Counts = 0;
  };
}

#endif
