/**
 * TextareaBuilder.h - "textarea": text input.
 * "placeholder", "text", "one_line" (bool), "password" (bool), "onClick" (action,
 * e.g. to pop up a keyboard).
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class TextareaBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* ta = lv_textarea_create(parent);
    if (node["placeholder"].is<const char*>())
      lv_textarea_set_placeholder_text(ta, node["placeholder"]);
    if (node["text"].is<const char*>())
      lv_textarea_set_text(
          ta, ctx.bindings().resolve(String(node["text"].as<const char*>())).c_str());
    if (node["one_line"] | false) lv_textarea_set_one_line(ta, true);
    if (node["password"] | false) lv_textarea_set_password_mode(ta, true);

    const char* action = node["onClick"] | (const char*)nullptr;
    if (action) ctx.attachAction(ta, String(action), LV_EVENT_CLICKED);
    return ta;
  }
};

}  // namespace ui
