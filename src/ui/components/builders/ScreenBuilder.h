/**
 * ScreenBuilder.h - "screen": root container that fills the parent, no border/scroll.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ScreenBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst, UiContext&) override {
    lv_obj_t* o = lv_obj_create(parent);
    lv_obj_set_size(o, lv_pct(100), lv_pct(100));
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_radius(o, 0, 0);
    lv_obj_clear_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    return o;
  }
};

}  // namespace ui
