/**
 * LGFX_Ili9341.h - LovyanGFX device for an SPI ILI9341 240x320 panel + XPT2046
 * touch (shared SPI bus), e.g. on an ESP32-C3.
 *
 * WARNING: the pins/timing below are REASONABLE SAMPLE VALUES ("on paper" board),
 * NOT validated on hardware. Adjust to the real wiring before flashing.
 *
 * Single responsibility: describe the display + touch hardware (SRP).
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX_Ili9341 : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel;
  lgfx::Bus_SPI       _bus;
  lgfx::Light_PWM     _light;
  lgfx::Touch_XPT2046 _touch;

 public:
  LGFX_Ili9341() {
    // --- SPI bus (ESP32-C3: use SPI2_HOST/FSPI) ---
    {
      auto cfg = _bus.config();
      cfg.spi_host    = SPI2_HOST;
      cfg.spi_mode    = 0;
      cfg.freq_write  = 40000000;
      cfg.freq_read   = 16000000;
      cfg.spi_3wire   = false;
      cfg.use_lock    = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = 4;
      cfg.pin_mosi = 6;
      cfg.pin_miso = 5;
      cfg.pin_dc   = 2;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    // --- ILI9341 panel 240x320 ---
    {
      auto cfg = _panel.config();
      cfg.pin_cs   = 7;
      cfg.pin_rst  = 3;
      cfg.pin_busy = -1;
      cfg.panel_width   = 240;
      cfg.panel_height  = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.readable = true;
      cfg.invert   = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }
    // --- PWM backlight ---
    {
      auto cfg = _light.config();
      cfg.pin_bl = 10;
      cfg.freq = 12000;
      cfg.pwm_channel = 1;
      _light.config(cfg);
      _panel.setLight(&_light);
    }
    // --- XPT2046 touch (shared SPI bus) ---
    {
      auto cfg = _touch.config();
      cfg.x_min = 0; cfg.x_max = 239;
      cfg.y_min = 0; cfg.y_max = 319;
      cfg.pin_int = 0;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.spi_host = SPI2_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 4;
      cfg.pin_mosi = 6;
      cfg.pin_miso = 5;
      cfg.pin_cs   = 1;
      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }

    setPanel(&_panel);
  }
};
