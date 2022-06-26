/*!
 * @file
 * @brief
 */

#ifndef tiny_hal_II2c_hpp
#define tiny_hal_II2c_hpp

#include <cstdint>

namespace tiny {
  class II2c {
   public:
    virtual ~II2c(){};

    virtual bool write(
      uint8_t address,
      bool prepare_for_restart,
      const uint8_t* buffer,
      uint16_t buffer_size) = 0;

    virtual bool read(
      uint8_t address,
      bool prepare_for_restart,
      uint8_t* buffer,
      uint16_t buffer_size) = 0;

    virtual void reset() = 0;
  };
}

#endif
