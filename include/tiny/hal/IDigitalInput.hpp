/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalInput_hpp
#define tiny_hal_IDigitalInput_hpp

namespace tiny {
  class IDigitalInput {
   public:
    virtual ~IDigitalInput(){};
    virtual bool read() = 0;
  };
}

#endif
