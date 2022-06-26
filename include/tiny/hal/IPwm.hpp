/*!
 * @file
 * @brief Abstract PWM channel.
 */

#ifndef tiny_hal_IPwm_hpp
#define tiny_hal_IPwm_hpp

#include <cstdint>

namespace tiny {
  class IPwm {
   public:
    typedef uint16_t DutyCycle;
    typedef uint16_t FrequencyHz;

   public:
    virtual ~IPwm(){};

    /*!
     * Sets the duty cycle. Range is normalized to 0-0xFFFF.
     */
    virtual void set_duty_cycle(DutyCycle duty_cycle) = 0;

    /*!
     * Sets the frequency.
     */
    virtual void set_frequency(FrequencyHz frequency) = 0;
  };
}

#endif
