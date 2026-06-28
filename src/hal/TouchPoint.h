/**
 * TouchPoint.h - One normalized touch point (in the display's logical coords).
 * Belongs to the HAL layer, so it does NOT depend on LVGL.
 */
#pragma once

#include <stdint.h>

namespace hal {

struct TouchPoint {
  int16_t x = 0;
  int16_t y = 0;
  bool pressed = false;
};

}  // namespace hal
