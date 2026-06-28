/**
 * UiParse.h - Helpers to convert JSON values into LVGL types (color, size,
 * alignment). Shared by UiFactory and the builders.
 */
#pragma once

#include <ArduinoJson.h>
#include <lvgl.h>

#include <Arduino.h>

namespace ui {

// "#RRGGBB" / "0xRRGGBB" / "RRGGBB" -> lv_color_t.
inline lv_color_t parseColor(const char* s, lv_color_t def) {
  if (!s || !*s) return def;
  if (*s == '#') {
    s++;
  } else if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
    s += 2;
  }
  uint32_t v = (uint32_t)strtoul(s, nullptr, 16);
  return lv_color_hex(v);
}

// int -> pixels; "100%" -> lv_pct; "content" -> LV_SIZE_CONTENT.
inline lv_coord_t parseDim(JsonVariantConst v, lv_coord_t def) {
  if (v.is<int>()) return (lv_coord_t)v.as<int>();
  if (v.is<const char*>()) {
    const char* s = v.as<const char*>();
    if (!s || !*s) return def;
    if (!strcmp(s, "content")) return LV_SIZE_CONTENT;
    size_t len = strlen(s);
    if (s[len - 1] == '%') return lv_pct(atoi(s));
    return (lv_coord_t)atoi(s);
  }
  return def;
}

inline lv_align_t parseAlign(const char* s, lv_align_t def) {
  if (!s) return def;
  struct Map { const char* k; lv_align_t v; };
  static const Map M[] = {
      {"center", LV_ALIGN_CENTER},
      {"top_left", LV_ALIGN_TOP_LEFT},
      {"top_mid", LV_ALIGN_TOP_MID},
      {"top_right", LV_ALIGN_TOP_RIGHT},
      {"bottom_left", LV_ALIGN_BOTTOM_LEFT},
      {"bottom_mid", LV_ALIGN_BOTTOM_MID},
      {"bottom_right", LV_ALIGN_BOTTOM_RIGHT},
      {"left_mid", LV_ALIGN_LEFT_MID},
      {"right_mid", LV_ALIGN_RIGHT_MID},
  };
  for (const auto& m : M)
    if (!strcmp(s, m.k)) return m.v;
  return def;
}

}  // namespace ui
