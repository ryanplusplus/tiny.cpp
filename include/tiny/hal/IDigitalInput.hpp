/*!
 * @file
 * @brief Abstract digital input.
 */

#ifndef tiny_hal_IDigitalInput_hpp
#define tiny_hal_IDigitalInput_hpp

namespace tiny {
  class IDigitalInput {
   public:
    virtual ~IDigitalInput(){};

    /*!
     * Reads the digital input.
     */
    virtual bool read() const = 0;
  };
}

#endif
