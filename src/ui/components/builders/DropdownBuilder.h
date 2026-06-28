/**
 * DropdownBuilder.h - "dropdown": "options" (array of strings), "selected" index,
 * "onChange" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class DropdownBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* dd = lv_dropdown_create(parent);

    String opts;
    for (JsonVariantConst o : node["options"].as<JsonArrayConst>()) {
      if (opts.length()) opts += "\n";
      opts += o.as<const char*>();
    }
    if (opts.length()) lv_dropdown_set_options(dd, opts.c_str());
    if (node["selected"].is<int>())
      lv_dropdown_set_selected(dd, node["selected"].as<int>());

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(dd, String(action), LV_EVENT_VALUE_CHANGED);
    return dd;
  }
};

}  // namespace ui
