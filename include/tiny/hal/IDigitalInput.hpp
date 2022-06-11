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
    virtual auto read() -> bool = 0;
  };
}

#endif
