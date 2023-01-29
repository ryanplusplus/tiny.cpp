/*!
 * @file
 * @brief Abstract analog input.
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

    /*!
     * Reads the analog input.
     *
     * The result is normalized to the full scale range of Counts.
     */
    virtual Counts read() const = 0;
  };
}

#endif
