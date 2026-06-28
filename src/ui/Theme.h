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
}

}  // namespace theme
}  // namespace ui
