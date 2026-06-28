/**
 * CrowPanelS3Board.h - IBoard for the CrowPanel ESP32-S3 5.0".
 * Assembles: LovyanGfxDisplay(LGFX_CrowPanel) + Gt911Touch.
 * This is the "template" for adding a board: just one file like this (Open/Closed).
 */
#pragma once

#include "../../drivers/display/LovyanGfxDisplay.h"
#include "../../drivers/touch/Gt911Touch.h"
#include "../../hal/IBoard.h"
#include "LGFX_CrowPanel.h"

namespace boards {

class CrowPanelS3Board : public hal::IBoard {
 public:
  const char* name() const override { return "CrowPanel ESP32-S3 5.0\""; }

  bool begin() override {
    bool ok = display_.begin();
    touch_.begin();
    return ok;
  }

  hal::IDisplay& display() override { return display_; }
  hal::ITouch& touch() override { return touch_; }

 private:
  static constexpr int16_t kW = 800;
  static constexpr int16_t kH = 480;

  LGFX_CrowPanel lcd_;
  drivers::LovyanGfxDisplay display_{lcd_, kW, kH};
  // GT911: SDA=19, SCL=20, INT/RST not wired (-1). Panel is rotated 180 -> invert both axes.
  drivers::Gt911Touch touch_{19, 20, -1, -1, kW, kH, /*invertX=*/true, /*invertY=*/true};
};

}  // namespace boards
