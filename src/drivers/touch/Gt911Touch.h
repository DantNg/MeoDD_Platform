/**
 * Gt911Touch.h - ITouch for the GT911 capacitive (I2C) touch, wrapping TAMC_GT911.
 * Rotation/axis-invert flags come from the board (e.g. CrowPanel is rotated 180
 * -> both axes inverted).
 *
 * Only compiled when DRIVER_GT911=1 (an env that adds the TAMC_GT911 library).
 */
#pragma once

#include "../../platform/Features.h"

#if DRIVER_GT911
#include <TAMC_GT911.h>

#include "../../hal/ITouch.h"

namespace drivers {

class Gt911Touch : public hal::ITouch {
 public:
  // sda/scl: I2C pins; intPin/rstPin: -1 if not connected.
  // invertX/invertY: flip axes to match the screen orientation.
  Gt911Touch(int sda, int scl, int intPin, int rstPin, uint16_t w, uint16_t h,
             bool invertX = false, bool invertY = false);

  bool begin() override;
  bool read(hal::TouchPoint& out) override;

 private:
  TAMC_GT911 dev_;
  uint16_t w_;
  uint16_t h_;
  bool invX_;
  bool invY_;
};

}  // namespace drivers
#endif  // DRIVER_GT911
