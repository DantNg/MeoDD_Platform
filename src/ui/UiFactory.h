/**
 * UiFactory.h - Reads a JSON schema and builds the LVGL widget tree.
 * Flow: look up a builder by node["type"] -> builder creates the widget ->
 * apply common properties -> recurse into children. UiFactory does not know any
 * concrete widget (everything via ComponentRegistry) -> adding a widget never
 * touches this file.
 */
#pragma once

#include <ArduinoJson.h>
#include <lvgl.h>

#include "UiContext.h"

namespace ui {

class UiFactory {
 public:
  explicit UiFactory(UiContext& ctx);

  // Build from a JSON string; returns the root object (or nullptr on error).
  lv_obj_t* buildFromJson(lv_obj_t* parent, const char* json);

  // Build from a single parsed node (recursive).
  lv_obj_t* buildNode(lv_obj_t* parent, JsonObjectConst node);

 private:
  void applyCommon(lv_obj_t* obj, JsonObjectConst node);

  UiContext& ctx_;
};

}  // namespace ui
