/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogOutput_h
#define tiny_hal_IAnalogOutput_h

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
