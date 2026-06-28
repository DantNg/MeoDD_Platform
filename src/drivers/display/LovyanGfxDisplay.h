/**
 * LovyanGfxDisplay.h
 * One IDisplay adapter SHARED by every LovyanGFX panel (RGB parallel or SPI).
 *
 * Key point: both the CrowPanel RGB panel and an ILI9341 SPI panel are an
 * lgfx::LGFX_Device with pushImageDMA -> a single adapter is enough. The hardware
 * differences live entirely in the panel config class (LGFX_CrowPanel /
 * LGFX_Ili9341) that the board provides.
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "../../hal/IDisplay.h"

namespace drivers {

class LovyanGfxDisplay : public hal::IDisplay {
 public:
  LovyanGfxDisplay(lgfx::LGFX_Device& dev, int16_t w, int16_t h)
      : dev_(dev), w_(w), h_(h) {}

  bool begin() override {
    if (!dev_.begin()) return false;
    dev_.setRotation(0);
    dev_.setSwapBytes(true);  // compensates for LV_COLOR_16_SWAP = 0 in lv_conf.h
    dev_.fillScreen(0x0000);
    return true;
  }

  int16_t width() const override { return w_; }
  int16_t height() const override { return h_; }

  void flush(int16_t x, int16_t y, int16_t w, int16_t h,
             const uint16_t* px) override {
    // pushImageDMA: non-blocking -> LVGL renders the other buffer while this one
    // is being pushed -> less tearing.
    dev_.startWrite();
    dev_.pushImageDMA(x, y, w, h, px);
    dev_.endWrite();
  }

  void setBacklight(uint8_t value) override { dev_.setBrightness(value); }

 private:
  lgfx::LGFX_Device& dev_;
  int16_t w_;
  int16_t h_;
};

}  // namespace drivers
