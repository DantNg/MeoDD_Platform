/**
 * ITouch.h - Touch abstraction. Returns a TouchPoint normalized to the display
 * orientation. GT911 (capacitive I2C) and XPT2046 (resistive SPI) both implement
 * this interface -> illustrates Open/Closed: a new touch type adds no edits to
 * existing code.
 */
#pragma once

#include "TouchPoint.h"

namespace hal {

class ITouch {
 public:
  virtual ~ITouch() = default;

  virtual bool begin() = 0;

  // Read the current state into `out`. Returns true while pressed.
  virtual bool read(TouchPoint& out) = 0;
};

// Empty touch for boards with no touch panel -> always "released".
class NullTouch : public ITouch {
 public:
  bool begin() override { return true; }
  bool read(TouchPoint& out) override {
    out.pressed = false;
    return false;
  }
};

}  // namespace hal
