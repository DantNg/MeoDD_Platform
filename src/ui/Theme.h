/**
 * Theme.h - Default color/font palette + registration of named styles into the
 * StyleRegistry. Call theme::registerStyles(ctx.styles()) once at UI startup.
 * Single source of truth for the look -> change once, the whole UI follows (DRY).
 */
#pragma once

#include <lvgl.h>

#include "StyleRegistry.h"

namespace ui {
namespace theme {

// --- Palette ---
inline lv_color_t bg()       { return lv_color_hex(0x0A0A12); }
inline lv_color_t card()     { return lv_color_hex(0x12121F); }
inline lv_color_t accent()   { return lv_color_hex(0x33FF66); }
inline lv_color_t accent2()  { return lv_color_hex(0x3399FF); }
inline lv_color_t text()     { return lv_color_hex(0xC8C8D0); }
inline lv_color_t textDim()  { return lv_color_hex(0x6E6E80); }
inline lv_color_t border()   { return lv_color_hex(0x2A2A40); }

inline void registerDashboardStyles(StyleRegistry& sr);

inline void registerStyles(StyleRegistry& sr) {
  {
    lv_style_t& s = sr.create("bg");
    lv_style_set_bg_color(&s, bg());
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_text_color(&s, text());
    lv_style_set_text_font(&s, &lv_font_montserrat_14);
  }
  {
    lv_style_t& s = sr.create("title");
    lv_style_set_text_color(&s, accent());
    lv_style_set_text_font(&s, &lv_font_montserrat_20);
  }
  {
    lv_style_t& s = sr.create("subtitle");
    lv_style_set_text_color(&s, textDim());
    lv_style_set_text_font(&s, &lv_font_montserrat_14);
  }
  {
    lv_style_t& s = sr.create("value");
    lv_style_set_text_color(&s, accent2());
    lv_style_set_text_font(&s, &lv_font_montserrat_14);
  }
  {
    lv_style_t& s = sr.create("card");
    lv_style_set_bg_color(&s, card());
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_radius(&s, 8);
    lv_style_set_border_width(&s, 1);
    lv_style_set_border_color(&s, border());
    lv_style_set_pad_all(&s, 12);
    lv_style_set_text_color(&s, text());
  }

  // --- Performance dashboard palette (matches the racing meter mockup) ---
  registerDashboardStyles(sr);
}

// Bright racing-green accent used across the dashboard.
inline lv_color_t raceGreen() { return lv_color_hex(0x35DD5B); }
inline lv_color_t raceBlue()  { return lv_color_hex(0x2F9BFF); }
inline lv_color_t numWhite()  { return lv_color_hex(0xF5F6FA); }
inline lv_color_t caption()   { return lv_color_hex(0x8A8A99); }
inline lv_color_t panelBg()   { return lv_color_hex(0x13141B); }
inline lv_color_t panelLine() { return lv_color_hex(0x23242E); }

inline void registerDashboardStyles(StyleRegistry& sr) {
  // Screen background: near-black like the device bezel render.
  {
    lv_style_t& s = sr.create("race_bg");
    lv_style_set_bg_color(&s, lv_color_hex(0x070810));
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_text_color(&s, text());
    lv_style_set_text_font(&s, &lv_font_montserrat_14);
    lv_style_set_pad_all(&s, 0);
  }
  // Rounded dark card.
  {
    lv_style_t& s = sr.create("rcard");
    lv_style_set_bg_color(&s, panelBg());
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_radius(&s, 10);
    lv_style_set_border_width(&s, 1);
    lv_style_set_border_color(&s, panelLine());
    lv_style_set_pad_all(&s, 14);
    lv_style_set_text_color(&s, text());
  }
  // Small uppercase caption (e.g. "GPS STATUS", "SPEED").
  {
    lv_style_t& s = sr.create("cap");
    lv_style_set_text_color(&s, caption());
    lv_style_set_text_font(&s, &lv_font_montserrat_12);
    lv_style_set_text_letter_space(&s, 1);
  }
  // Section heading (e.g. "PERFORMANCE RESULTS").
  {
    lv_style_t& s = sr.create("sect");
    lv_style_set_text_color(&s, numWhite());
    lv_style_set_text_font(&s, &lv_font_montserrat_16);
  }
  // Bold-ish white status line (e.g. "READY TO START").
  {
    lv_style_t& s = sr.create("ready");
    lv_style_set_text_color(&s, lv_color_white());
    lv_style_set_text_font(&s, &lv_font_montserrat_20);
  }
  // Result value, white.
  {
    lv_style_t& s = sr.create("val");
    lv_style_set_text_color(&s, numWhite());
    lv_style_set_text_font(&s, &lv_font_montserrat_20);
  }
  // Result value, green (the headline 0-60 time).
  {
    lv_style_t& s = sr.create("val_green");
    lv_style_set_text_color(&s, raceGreen());
    lv_style_set_text_font(&s, &lv_font_montserrat_20);
  }
  // Extra-large number (the live speed readout).
  {
    lv_style_t& s = sr.create("num_xl");
    lv_style_set_text_color(&s, lv_color_white());
    lv_style_set_text_font(&s, &lv_font_montserrat_48);
  }
  // Large number (top speed, horsepower).
  {
    lv_style_t& s = sr.create("num_lg");
    lv_style_set_text_color(&s, lv_color_white());
    lv_style_set_text_font(&s, &lv_font_montserrat_40);
  }
  // Unit suffix next to a big number (mph, hp).
  {
    lv_style_t& s = sr.create("unit");
    lv_style_set_text_color(&s, caption());
    lv_style_set_text_font(&s, &lv_font_montserrat_16);
  }
  // Small pill badge ("RUN #12").
  {
    lv_style_t& s = sr.create("badge");
    lv_style_set_bg_color(&s, lv_color_hex(0x20212B));
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_radius(&s, 6);
    lv_style_set_text_color(&s, lv_color_hex(0x9AA0AE));
    lv_style_set_text_font(&s, &lv_font_montserrat_12);
    lv_style_set_pad_hor(&s, 8);
    lv_style_set_pad_ver(&s, 4);
  }
  // Transparent overlay for the START label (the green glow is in the bg image).
  {
    lv_style_t& s = sr.create("start_text");
    lv_style_set_bg_opa(&s, LV_OPA_TRANSP);
    lv_style_set_border_width(&s, 0);
    lv_style_set_shadow_width(&s, 0);
    lv_style_set_text_color(&s, lv_color_white());
    lv_style_set_text_font(&s, &lv_font_montserrat_28);
  }
  // Big green START button with a soft glow.
  {
    lv_style_t& s = sr.create("start_btn");
    lv_style_set_bg_color(&s, lv_color_hex(0x2BC24A));
    lv_style_set_bg_opa(&s, LV_OPA_COVER);
    lv_style_set_radius(&s, 14);
    lv_style_set_border_width(&s, 0);
    lv_style_set_text_color(&s, lv_color_white());
    lv_style_set_text_font(&s, &lv_font_montserrat_28);
    lv_style_set_shadow_color(&s, raceGreen());
    lv_style_set_shadow_width(&s, 28);
    lv_style_set_shadow_opa(&s, LV_OPA_50);
  }
}

}  // namespace theme
}  // namespace ui
