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
    virtual auto write(Channel channel, bool state) -> void;
  };
}

#endif
