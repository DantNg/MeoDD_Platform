/**
 * UiFactory.cpp - JSON schema -> LVGL widget tree.
 */
#include "UiFactory.h"

#include <Arduino.h>

#include "UiParse.h"

namespace ui {

UiFactory::UiFactory(UiContext& ctx) : ctx_(ctx) {
  // Let builders recurse into nested nodes through the context.
  ctx_.setNodeBuilder(
      [this](lv_obj_t* parent, JsonObjectConst node) {
        return this->buildNode(parent, node);
      });
}

lv_obj_t* UiFactory::buildFromJson(lv_obj_t* parent, const char* json) {
  JsonDocument doc;
  // Raise the nesting limit (default 10): nested containers/panels easily go
  // deeper than that (screen > tabview > tab > card > row > widget).
  DeserializationError err =
      deserializeJson(doc, json, DeserializationOption::NestingLimit(64));
  if (err) {
    Serial.printf("[UI] JSON error: %s\n", err.c_str());
    return nullptr;
  }
  return buildNode(parent, doc.as<JsonObjectConst>());
}

lv_obj_t* UiFactory::buildNode(lv_obj_t* parent, JsonObjectConst node) {
  const char* type = node["type"] | "";
  IComponentBuilder* builder = ctx_.components().find(type);
  if (!builder) {
    Serial.printf("[UI] unknown type: '%s'\n", type);
    return nullptr;
  }

  lv_obj_t* obj = builder->build(parent, node, ctx_);
  if (!obj) return nullptr;

  applyCommon(obj, node);

  // Recurse into children unless the builder already consumed them.
  if (!builder->buildsOwnChildren()) {
    for (JsonVariantConst child : node["children"].as<JsonArrayConst>()) {
      buildNode(obj, child.as<JsonObjectConst>());
    }
  }
  return obj;
}

void UiFactory::applyCommon(lv_obj_t* o, JsonObjectConst node) {
  // 1) Named styles (a string or an array of names).
  JsonVariantConst st = node["style"];
  if (st.is<const char*>()) {
    if (lv_style_t* s = ctx_.styles().find(st.as<const char*>()))
      lv_obj_add_style(o, s, 0);
  } else if (st.is<JsonArrayConst>()) {
    for (JsonVariantConst e : st.as<JsonArrayConst>())
      if (lv_style_t* s = ctx_.styles().find(e.as<const char*>()))
        lv_obj_add_style(o, s, 0);
  }

  // 2) Inline overrides.
  if (node["bg"].is<const char*>()) {
    lv_obj_set_style_bg_color(o, parseColor(node["bg"], lv_color_black()), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
  }
  if (node["text_color"].is<const char*>())
    lv_obj_set_style_text_color(o, parseColor(node["text_color"], lv_color_white()), 0);
  if (node["radius"].is<int>())
    lv_obj_set_style_radius(o, node["radius"].as<int>(), 0);
  if (node["pad"].is<int>())
    lv_obj_set_style_pad_all(o, node["pad"].as<int>(), 0);
  if (node["border"].is<int>())
    lv_obj_set_style_border_width(o, node["border"].as<int>(), 0);

  // 3) Size.
  if (!node["width"].isNull())
    lv_obj_set_width(o, parseDim(node["width"], lv_obj_get_width(o)));
  if (!node["height"].isNull())
    lv_obj_set_height(o, parseDim(node["height"], lv_obj_get_height(o)));

  // 4) Flex layout.
  if (node["flex"].is<const char*>()) {
    const char* f = node["flex"];
    if (!strcmp(f, "row"))
      lv_obj_set_flex_flow(o, LV_FLEX_FLOW_ROW);
    else if (!strcmp(f, "column") || !strcmp(f, "col"))
      lv_obj_set_flex_flow(o, LV_FLEX_FLOW_COLUMN);
    if (node["gap"].is<int>()) {
      const int g = node["gap"];
      lv_obj_set_style_pad_row(o, g, 0);
      lv_obj_set_style_pad_column(o, g, 0);
    }
  }
  if (node["grow"].is<int>()) lv_obj_set_flex_grow(o, node["grow"].as<int>());

  // 5) Position / alignment.
  if (node["align"].is<const char*>())
    lv_obj_set_align(o, parseAlign(node["align"], LV_ALIGN_DEFAULT));
  if (node["x"].is<int>() || node["y"].is<int>())
    lv_obj_set_pos(o, node["x"] | 0, node["y"] | 0);

  // 6) Id for later reference.
  if (node["id"].is<const char*>())
    ctx_.registerId(node["id"].as<const char*>(), o);

  // 7) Visibility.
  if (node["hidden"] | false) lv_obj_add_flag(o, LV_OBJ_FLAG_HIDDEN);
}

}  // namespace ui
