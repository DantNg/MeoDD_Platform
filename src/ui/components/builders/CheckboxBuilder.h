/**
 * CheckboxBuilder.h - "checkbox": "text" + "checked" + "onChange" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class CheckboxBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* cb = lv_checkbox_create(parent);
    const char* text = node["text"] | "";
    lv_checkbox_set_text(cb, ctx.bindings().resolve(String(text)).c_str());
    if (node["checked"] | false) lv_obj_add_state(cb, LV_STATE_CHECKED);

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(cb, String(action), LV_EVENT_VALUE_CHANGED);
    return cb;
  }
};

}  // namespace ui
