/*!
 * @file
 * @brief
 */

#ifndef IDigitalInputGroup_h
#define IDigitalInputGroup_h

#include <cstdint>

namespace tiny {
  class IDigitalInputGroup {
   public:
    typedef uint8_t Channel;

   public:
    virtual ~IDigitalInputGroup(){};
    virtual auto read(Channel channel) -> bool = 0;
  };
}

#endif
