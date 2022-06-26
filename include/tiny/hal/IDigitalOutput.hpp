/*!
 * @file
 * @brief Abstract digital output.
 */

#ifndef tiny_hal_IDigitalOutput_hpp
#define tiny_hal_IDigitalOutput_hpp

namespace tiny {
  class IDigitalOutput {
   public:
    virtual ~IDigitalOutput(){};

    /*!
     * Writes the digital output.
     */
    virtual void write(bool state) = 0;
  };
}

#endif
