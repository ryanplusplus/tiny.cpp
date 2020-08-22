/*!
 * @file
 * @brief
 */

#ifndef IPwm_h
#define IPwm_h

#include <cstdint>

namespace tiny {
  class IPwm {
   public:
    typedef uint16_t DutyCycle;
    typedef uint16_t FrequencyHz;

   public:
    virtual ~IPwm(){};
    auto virtual set_duty_cycle(DutyCycle duty_cycle) -> void = 0;
    auto virtual set_frequency(FrequencyHz frequency) -> void = 0;
  };
}

#endif
