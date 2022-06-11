/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogOutput_hpp
#define tiny_hal_IAnalogOutput_hpp

#include <cstdint>

namespace tiny {
  class IAnalogOutput {
   public:
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogOutput(){};
    virtual auto write(Counts counts) -> void = 0;
  };
}

#endif
