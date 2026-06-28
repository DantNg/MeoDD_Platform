/**
 * ArcBuilder.h - "arc": circular value control with min/max/value + "onChange".
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ArcBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* arc = lv_arc_create(parent);
    lv_arc_set_range(arc, node["min"] | 0, node["max"] | 100);
    lv_arc_set_value(arc, node["value"] | 0);

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(arc, String(action), LV_EVENT_VALUE_CHANGED);
    return arc;
  }
};

}  // namespace ui
