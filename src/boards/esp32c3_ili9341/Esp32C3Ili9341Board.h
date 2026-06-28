/**
 * Esp32C3Ili9341Board.h - IBoard for ESP32-C3 + ILI9341 SPI + XPT2046.
 *
 * Demonstrates Open/Closed: adding this board edits NO existing file
 * (LvglPort/UI/services stay the same) - only new files + one BoardRegistry
 * branch. Reuses LovyanGfxDisplay (shared with the RGB board) and LovyanTouch.
 *
 * "On paper" board: not validated on real hardware.
 */
#pragma once

#include "../../drivers/display/LovyanGfxDisplay.h"
#include "../../drivers/touch/LovyanTouch.h"
#include "../../hal/IBoard.h"
#include "LGFX_Ili9341.h"

namespace boards {

class Esp32C3Ili9341Board : public hal::IBoard {
 public:
  const char* name() const override { return "ESP32-C3 + ILI9341 SPI"; }

  bool begin() override {
    bool ok = display_.begin();
    touch_.begin();
    return ok;
  }

  hal::IDisplay& display() override { return display_; }
  hal::ITouch& touch() override { return touch_; }

 private:
  static constexpr int16_t kW = 240;
  static constexpr int16_t kH = 320;

  LGFX_Ili9341 lcd_;
  drivers::LovyanGfxDisplay display_{lcd_, kW, kH};
  drivers::LovyanTouch touch_{lcd_};
};

}  // namespace boards
