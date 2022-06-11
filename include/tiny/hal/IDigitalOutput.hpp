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
    virtual auto write(bool state) -> void = 0;
  };
}

#endif
