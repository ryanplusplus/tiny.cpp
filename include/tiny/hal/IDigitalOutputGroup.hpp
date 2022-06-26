/*!
 * @file
 * @brief Abstract group of digital outputs.
 */

#ifndef tiny_hal_IDigitalOutputGroup_hpp
#define tiny_hal_IDigitalOutputGroup_hpp

#include <cstdint>

namespace tiny {
  class IDigitalOutputGroup {
   public:
    typedef uint8_t Channel;

   public:
    virtual ~IDigitalOutputGroup(){};

    /*!
     * Writes the digital output channel.
     */
    virtual void write(Channel channel, bool state);
  };
}

#endif
