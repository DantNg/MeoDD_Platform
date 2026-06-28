/**
 * SliderBuilder.h - "slider": min/max/value + "onChange" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class SliderBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* o = lv_slider_create(parent);
    const int mn = node["min"] | 0;
    const int mx = node["max"] | 100;
    const int val = node["value"] | 0;
    lv_slider_set_range(o, mn, mx);
    lv_slider_set_value(o, val, LV_ANIM_OFF);

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(o, String(action), LV_EVENT_VALUE_CHANGED);
    return o;
  }
};

}  // namespace ui
