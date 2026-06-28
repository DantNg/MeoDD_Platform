/**
 * BarBuilder.h - "bar": progress bar with min/max/value.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class BarBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node, UiContext&) override {
    lv_obj_t* o = lv_bar_create(parent);
    const int mn = node["min"] | 0;
    const int mx = node["max"] | 100;
    const int val = node["value"] | 0;
    lv_bar_set_range(o, mn, mx);
    lv_bar_set_value(o, val, LV_ANIM_OFF);
    return o;
  }
};

}  // namespace ui
