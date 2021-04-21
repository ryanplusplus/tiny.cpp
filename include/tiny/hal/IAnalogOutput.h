/*!
 * @file
 * @brief
 */

#ifndef IAnalogOutput_h
#define IAnalogOutput_h

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
