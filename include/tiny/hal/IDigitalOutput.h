/*!
 * @file
 * @brief
 */

#ifndef IDigitalOutput_h
#define IDigitalOutput_h

namespace tiny
{
  class IDigitalOutput
  {
   public:
    virtual ~IDigitalOutput(){};
    auto virtual write(bool state) -> void = 0;
  };
}

#endif
