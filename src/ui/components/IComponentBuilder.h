/**
 * IComponentBuilder.h - Contract for building ONE widget type from a JSON node.
 *
 * Adding a new widget to the schema = write a class implementing this interface
 * and register it in ComponentRegistry. UiFactory does NOT change (Open/Closed).
 */
#pragma once

#include <ArduinoJson.h>
#include <lvgl.h>

namespace ui {

class UiContext;  // forward declared to avoid an include cycle

class IComponentBuilder {
 public:
  virtual ~IComponentBuilder() = default;

  // Create the widget from `node` and attach it to `parent`. Return the new
  // object (or nullptr). Common properties (size/align/style/flex...) are applied
  // by UiFactory afterwards; the builder only handles widget-specific bits
  // (text, range, options, src...).
  virtual lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                          UiContext& ctx) = 0;

  // Return true if the builder consumes node["children"] itself (e.g. a tabview
  // routes children into tab pages). Then UiFactory will NOT recurse into them.
  virtual bool buildsOwnChildren() const { return false; }
};

}  // namespace ui
