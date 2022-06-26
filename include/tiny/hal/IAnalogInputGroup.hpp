/*!
 * @file
 * @brief Abstract group of analog inputs.
 */

#ifndef tiny_hal_IAnalogInputGroup_hpp
#define tiny_hal_IAnalogInputGroup_hpp

#include <cstdint>

namespace tiny {
  class IAnalogInputGroup {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogInputGroup(){};

    /*!
     * Reads the analog input channel.
     *
     * The result is normalized to the full scale range of Counts.
     */
    virtual Counts read(Channel channel) = 0;
  };
}

#endif
