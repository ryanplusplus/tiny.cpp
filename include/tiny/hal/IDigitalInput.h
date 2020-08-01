/*!
 * @file
 * @brief
 */

#ifndef IDigitalInput_h
#define IDigitalInput_h

namespace tiny
{
  class IDigitalInput
  {
   public:
    virtual ~IDigitalInput(){};
    auto virtual read() -> bool = 0;
  };
}

#endif
