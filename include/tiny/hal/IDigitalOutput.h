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
    auto virtual write(bool state) -> void = 0;
  };
}

#endif
