/**
 * LovyanTouch.h - ITouch using the touch panel INTEGRATED in a LovyanGFX device
 * (XPT2046, FT5x06, GT911...). No separate touch library is needed: the touch is
 * configured inside the board's LGFX class, and here we just read it via
 * dev_.getTouch().
 *
 * Header-only -> compiled only when a board includes it (no cross-dependency).
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "../../hal/ITouch.h"

namespace drivers {

class LovyanTouch : public hal::ITouch {
 public:
  explicit LovyanTouch(lgfx::LGFX_Device& dev) : dev_(dev) {}

  bool begin() override { return true; }  // touch is initialized in dev_.begin()

  bool read(hal::TouchPoint& out) override {
    int32_t x = 0, y = 0;
    if (dev_.getTouch(&x, &y)) {
      out.x = (int16_t)x;
      out.y = (int16_t)y;
      out.pressed = true;
      return true;
    }
    out.pressed = false;
    return false;
  }

 private:
  lgfx::LGFX_Device& dev_;
};

}  // namespace drivers
