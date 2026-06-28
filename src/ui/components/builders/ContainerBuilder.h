/**
 * ContainerBuilder.h - "container"/"panel": groups children, transparent and
 * borderless by default. Layout is driven by the common "flex"/"gap" props.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ContainerBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst, UiContext&) override {
    lv_obj_t* o = lv_obj_create(parent);
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_clear_flag(o, LV_OBJ_FLAG_SCROLLABLE);
    return o;
  }
};

}  // namespace ui
