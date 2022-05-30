/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogInput_h
#define tiny_hal_IAnalogInput_h

#include <cstdint>

namespace {
  class IAnalogInput {
   public:
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogInput(){};
    virtual auto read() -> Counts = 0;
  };
}

#endif
