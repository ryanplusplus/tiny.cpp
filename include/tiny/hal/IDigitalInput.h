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
    auto virtual read() -> bool = 0;
  };
}

#endif
