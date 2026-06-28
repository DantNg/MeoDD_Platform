/**
 * ButtonBuilder.h - "button": a button, optionally with a "text" label and an
 * "onClick" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ButtonBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* btn = lv_btn_create(parent);

    const char* text = node["text"] | (const char*)nullptr;
    if (text) {
      lv_obj_t* lbl = lv_label_create(btn);
      lv_label_set_text(lbl, ctx.bindings().resolve(String(text)).c_str());
      lv_obj_center(lbl);
    }

    const char* action = node["onClick"] | (const char*)nullptr;
    if (action) ctx.attachAction(btn, String(action), LV_EVENT_CLICKED);

    return btn;
  }
};

}  // namespace ui
