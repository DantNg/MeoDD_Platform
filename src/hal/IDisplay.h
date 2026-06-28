/**
 * IDisplay.h - Display abstraction at the RAW PIXEL level (RGB565), LVGL-agnostic.
 * LvglPort is the only place that wires this into LVGL. Add a new display = write
 * a class implementing IDisplay (Open/Closed).
 */
#pragma once

#include <stdint.h>

namespace hal {

class IDisplay {
 public:
  virtual ~IDisplay() = default;

  // Initialize the display hardware. Return false on failure.
  virtual bool begin() = 0;

  virtual int16_t width() const = 0;
  virtual int16_t height() const = 0;

  // Push a w*h block of RGB565 pixels to the screen at (x, y).
  virtual void flush(int16_t x, int16_t y, int16_t w, int16_t h,
                     const uint16_t* pixels) = 0;

  // Backlight 0..255 (boards without backlight control just ignore it).
  virtual void setBacklight(uint8_t value) = 0;
};

}  // namespace hal
