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
    virtual void write(Counts counts) = 0;
  };
}

#endif
