/*!
 * @file
 * @brief Abstraction for getting the current system time. This can be implemented
 * using something like Arduino's millis() or by using a system tick interrupt to
 * keep time.
 */

#ifndef tiny_ITimeSource_hpp
#define tiny_ITimeSource_hpp

#include <cstdint>

namespace tiny {
  class ITimeSource {
   public:
    typedef uint16_t TickCount;

   public:
    virtual ~ITimeSource(){};
    virtual TickCount ticks() = 0;
  };
}

#endif
