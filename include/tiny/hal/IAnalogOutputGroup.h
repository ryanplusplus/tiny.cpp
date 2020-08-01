/*!
 * @file
 * @brief
 */

#ifndef IAnalogOutputGroup_h
#define IAnalogOutputGroup_h

#include <cstdint>

namespace tiny
{
  class IAnalogOutputGroup
  {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogOutputGroup(){};
    auto virtual write(Channel channel, Counts counts) -> void = 0;
  };
}

#endif
