/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalOutputGroup_hpp
#define tiny_hal_IDigitalOutputGroup_hpp

#include <cstdint>

namespace tiny {
  class IDigitalOutputGroup {
   public:
    typedef uint8_t Channel;

   public:
    virtual ~IDigitalOutputGroup(){};
    virtual void write(Channel channel, bool state);
  };
}

#endif
