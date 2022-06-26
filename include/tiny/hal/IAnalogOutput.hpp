/*!
 * @file
 * @brief Abstract group of analog outputs.
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

    /*!
     * Writes the analog output channel.
     *
     * The output counts are normalized to the full scale range of Counts.
     */
    virtual void write(Counts counts) = 0;
  };
}

#endif
