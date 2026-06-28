/**
 * SwitchBuilder.h - "switch": "checked" state + "onChange" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class SwitchBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* o = lv_switch_create(parent);
    if (node["checked"] | false) lv_obj_add_state(o, LV_STATE_CHECKED);

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(o, String(action), LV_EVENT_VALUE_CHANGED);
    return o;
  }
};

}  // namespace ui
