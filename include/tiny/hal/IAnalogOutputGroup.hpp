/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogOutputGroup_hpp
#define tiny_hal_IAnalogOutputGroup_hpp

#include <cstdint>

namespace tiny {
  class IAnalogOutputGroup {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogOutputGroup(){};
    virtual void write(Channel channel, Counts counts) = 0;
  };
}

#endif
