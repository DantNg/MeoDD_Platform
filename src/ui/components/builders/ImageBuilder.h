/**
 * ImageBuilder.h - "image": "src" is a path (needs an FS driver) or an LVGL symbol.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class ImageBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node, UiContext&) override {
    lv_obj_t* o = lv_img_create(parent);
    const char* src = node["src"] | (const char*)nullptr;
    if (src) lv_img_set_src(o, src);
    return o;
  }
};

}  // namespace ui
