/*!
 * @file
 * @brief Abstract analog output.
 */

#ifndef tiny_hal_IAnalogOutputGroup_hpp
#define tiny_hal_IAnalogOutputGroup_hpp

#include <cstdint>

namespace tiny {
  class IAnalogOutputGroup {
   public:
    typedef uint8_t Channel;
    typedef uint16_t Counts;

   public:
    virtual ~IAnalogOutputGroup(){};

    /*!
     * Writes the analog output.
     *
     * The output counts are normalized to the full scale range of Counts.
     */
    virtual void write(Channel channel, Counts counts) = 0;
  };
}

#endif
