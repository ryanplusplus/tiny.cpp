/*!
 * @file
 * @brief
 */

#ifndef IAnalogInput_h
#define IAnalogInput_h

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
