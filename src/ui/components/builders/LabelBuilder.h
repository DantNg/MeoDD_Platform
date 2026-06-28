/**
 * LabelBuilder.h - "label": text may contain "{key}" binding tokens.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class LabelBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* o = lv_label_create(parent);
    const char* text = node["text"] | "";
    lv_label_set_text(o, ctx.bindings().resolve(String(text)).c_str());
    if (node["wrap"] | false) lv_label_set_long_mode(o, LV_LABEL_LONG_WRAP);
    // "live": re-resolve the {token} text periodically (driven by Store updates).
    if (node["live"] | false) ctx.trackLive(o, String(text));
    return o;
  }
};

}  // namespace ui
