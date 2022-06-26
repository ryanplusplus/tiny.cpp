/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalOutput_hpp
#define tiny_hal_IDigitalOutput_hpp

namespace tiny {
  class IDigitalOutput {
   public:
    virtual ~IDigitalOutput(){};
    virtual void write(bool state) = 0;
  };
}

#endif
