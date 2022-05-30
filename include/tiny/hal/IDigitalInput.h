/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_IDigitalInput_h
#define tiny_hal_IDigitalInput_h

namespace tiny {
  class IDigitalInput {
   public:
    virtual ~IDigitalInput(){};
    virtual auto read() -> bool = 0;
  };
}

#endif
