/**
 * LGFX_CrowPanel.h
 * LovyanGFX device for the CrowPanel ESP32-S3 5.0" panel (800x480 RGB).
 * Pins & timing taken from Elecrow's reference configuration.
 *
 * Single responsibility of this file: describe the display hardware (SRP).
 */
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
// The ESP32-S3 RGB Panel/Bus are not pulled in by default -> include them here.
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>

class LGFX_CrowPanel : public lgfx::LGFX_Device {
  lgfx::Bus_RGB   _bus;
  lgfx::Panel_RGB _panel;
  lgfx::Light_PWM _light;

 public:
  LGFX_CrowPanel() {
    // --- Panel: logical/physical size ---
    {
      auto cfg = _panel.config();
      cfg.memory_width  = 800;
      cfg.memory_height = 480;
      cfg.panel_width   = 800;
      cfg.panel_height  = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel.config(cfg);
    }
    // --- Panel detail: framebuffer in PSRAM (required for RGB 800x480) ---
    {
      auto cfg = _panel.config_detail();
      cfg.use_psram = 1;
      _panel.config_detail(cfg);
    }
    // --- RGB16 bus: pin order B0..B4, G0..G5, R0..R4 + sync + timing ---
    {
      auto cfg = _bus.config();
      cfg.panel = &_panel;

      // Blue (B0..B4)
      cfg.pin_d0  = GPIO_NUM_8;
      cfg.pin_d1  = GPIO_NUM_3;
      cfg.pin_d2  = GPIO_NUM_46;
      cfg.pin_d3  = GPIO_NUM_9;
      cfg.pin_d4  = GPIO_NUM_1;
      // Green (G0..G5)
      cfg.pin_d5  = GPIO_NUM_5;
      cfg.pin_d6  = GPIO_NUM_6;
      cfg.pin_d7  = GPIO_NUM_7;
      cfg.pin_d8  = GPIO_NUM_15;
      cfg.pin_d9  = GPIO_NUM_16;
      cfg.pin_d10 = GPIO_NUM_4;
      // Red (R0..R4)
      cfg.pin_d11 = GPIO_NUM_45;
      cfg.pin_d12 = GPIO_NUM_48;
      cfg.pin_d13 = GPIO_NUM_47;
      cfg.pin_d14 = GPIO_NUM_21;
      cfg.pin_d15 = GPIO_NUM_14;

      cfg.pin_henable = GPIO_NUM_40;
      cfg.pin_vsync   = GPIO_NUM_41;
      cfg.pin_hsync   = GPIO_NUM_39;
      cfg.pin_pclk    = GPIO_NUM_0;
      cfg.freq_write  = 15000000;  // 15 MHz pixel clock

      cfg.hsync_polarity    = 0;
      cfg.hsync_front_porch = 8;
      cfg.hsync_pulse_width = 4;
      cfg.hsync_back_porch  = 43;
      cfg.vsync_polarity    = 0;
      cfg.vsync_front_porch = 8;
      cfg.vsync_pulse_width = 4;
      cfg.vsync_back_porch  = 12;
      cfg.pclk_active_neg   = 1;
      cfg.de_idle_high      = 0;
      cfg.pclk_idle_high    = 0;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    // --- PWM backlight on GPIO2 ---
    {
      auto cfg = _light.config();
      cfg.pin_bl = GPIO_NUM_2;
      _light.config(cfg);
      _panel.light(&_light);
    }

    setPanel(&_panel);
  }
};
