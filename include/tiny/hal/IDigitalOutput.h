/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalOutput_h
#define tiny_hal_IDigitalOutput_h

namespace tiny {
  class IDigitalOutput {
   public:
    virtual ~IDigitalOutput(){};
    virtual auto write(bool state) -> void = 0;
  };
}

#endif
