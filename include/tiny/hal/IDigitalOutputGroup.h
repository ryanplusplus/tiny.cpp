/*!
 * @file
 * @brief
 */

#ifndef IDigitalOutputGroup_h
#define IDigitalOutputGroup_h

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
