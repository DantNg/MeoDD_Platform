/**
 * StyleRegistry.h - Registry of named lv_style_t ("bg","title","card"...).
 * The schema references styles by name via "style"; the values are defined in
 * code (Theme) -> change the look in one place, the whole UI follows.
 */
#pragma once

#include <Arduino.h>
#include <lvgl.h>

#include <map>

namespace ui {

class StyleRegistry {
 public:
  // Create (or reset) a style named `name`; return a reference to configure it.
  lv_style_t& create(const String& name) {
    lv_style_t& s = styles_[name];
    lv_style_init(&s);
    return s;
  }

  lv_style_t* find(const String& name) {
    auto it = styles_.find(name);
    return it == styles_.end() ? nullptr : &it->second;
  }

  bool has(const String& name) const { return styles_.count(name) > 0; }

 private:
  // std::map is node-based -> lv_style_t addresses stay stable (LVGL keeps the
  // pointer).
  std::map<String, lv_style_t> styles_;
};

}  // namespace ui
