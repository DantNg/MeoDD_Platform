/**
 * ListBuilder.h - "list": a scrollable list of buttons.
 * "items": array of { "text": "...", "onClick": "action" }.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ListBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* list = lv_list_create(parent);
    // Smooth scrolling on slow panels: track the finger 1:1, no overshoot.
    lv_obj_clear_flag(list,
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);
    for (JsonVariantConst itemv : node["items"].as<JsonArrayConst>()) {
      JsonObjectConst item = itemv.as<JsonObjectConst>();
      const char* text = item["text"] | "";
      lv_obj_t* btn = lv_list_add_btn(
          list, NULL, ctx.bindings().resolve(String(text)).c_str());
      const char* action = item["onClick"] | (const char*)nullptr;
      if (action) ctx.attachAction(btn, String(action), LV_EVENT_CLICKED);
    }
    return list;
  }
};

}  // namespace ui
