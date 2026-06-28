/**
 * KeyboardBuilder.h - "keyboard": an on-screen keyboard. Usually placed once at
 * the root (hidden), then shown/hidden by actions when a textarea is tapped.
 * "onReady"/"onCancel" are actions (e.g. "kb.hide").
 *
 * Link it to a textarea from a C++ action with lv_keyboard_set_textarea(kb, ta).
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class KeyboardBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* kb = lv_keyboard_create(parent);

    const char* onReady = node["onReady"] | (const char*)nullptr;
    if (onReady) ctx.attachAction(kb, String(onReady), LV_EVENT_READY);
    const char* onCancel = node["onCancel"] | (const char*)nullptr;
    if (onCancel) ctx.attachAction(kb, String(onCancel), LV_EVENT_CANCEL);
    return kb;
  }
};

}  // namespace ui
