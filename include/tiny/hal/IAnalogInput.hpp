/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IAnalogInput_hpp
#define tiny_hal_IAnalogInput_hpp

#include <cstdint>

namespace {
  class IAnalogInput {
   public:
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogInput(){};
    virtual Counts read() = 0;
  };
}

#endif
