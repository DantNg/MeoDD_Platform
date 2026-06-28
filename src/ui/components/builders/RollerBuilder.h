/**
 * RollerBuilder.h - "roller": "options" (array of strings), "selected" index,
 * "onChange" action.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class RollerBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* roller = lv_roller_create(parent);

    String opts;
    for (JsonVariantConst o : node["options"].as<JsonArrayConst>()) {
      if (opts.length()) opts += "\n";
      opts += o.as<const char*>();
    }
    if (opts.length())
      lv_roller_set_options(roller, opts.c_str(), LV_ROLLER_MODE_NORMAL);
    if (node["selected"].is<int>())
      lv_roller_set_selected(roller, node["selected"].as<int>(), LV_ANIM_OFF);

    const char* action = node["onChange"] | (const char*)nullptr;
    if (action) ctx.attachAction(roller, String(action), LV_EVENT_VALUE_CHANGED);
    return roller;
  }
};

}  // namespace ui
