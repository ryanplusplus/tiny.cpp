/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalInputGroup_hpp
#define tiny_hal_IDigitalInputGroup_hpp

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
