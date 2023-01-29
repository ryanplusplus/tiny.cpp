/*!
 * @file
 * @brief Abstract group of digital inputs.
 */

#ifndef tiny_hal_IDigitalInputGroup_hpp
#define tiny_hal_IDigitalInputGroup_hpp

#include <cstdint>

namespace tiny {
  class IDigitalInputGroup {
   public:
    typedef uint8_t Channel;

   public:
    virtual ~IDigitalInputGroup(){};

    /*!
     * Reads the digital input channel.
     */
    virtual bool read(Channel channel) const = 0;
  };
}

#endif
