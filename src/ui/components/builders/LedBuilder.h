/**
 * LedBuilder.h - "led": indicator with "color", "brightness" (0..255), "on".
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"
#include "../../UiParse.h"

namespace ui {

class LedBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node, UiContext&) override {
    lv_obj_t* led = lv_led_create(parent);
    if (node["color"].is<const char*>())
      lv_led_set_color(led, parseColor(node["color"], lv_color_white()));
    lv_led_set_brightness(led, node["brightness"] | 255);
    if (node["on"] | true)
      lv_led_on(led);
    else
      lv_led_off(led);
    return led;
  }
};

}  // namespace ui
