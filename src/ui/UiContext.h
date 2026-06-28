/**
 * UiContext.h - Bundles the engine registries in one place: component + style +
 * action + binding. Passed to UiFactory and to every builder.
 *
 * It also exposes buildNode() (wired by UiFactory) so a builder can recursively
 * build nested nodes - e.g. a tabview building its tab pages.
 */
#pragma once

#include <ArduinoJson.h>
#include <Arduino.h>
#include <lvgl.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "ActionBus.h"
#include "Bindings.h"
#include "StyleRegistry.h"
#include "components/ComponentRegistry.h"

namespace ui {

class UiContext {
 public:
  using NodeBuilder = std::function<lv_obj_t*(lv_obj_t*, JsonObjectConst)>;

  ComponentRegistry& components() { return components_; }
  StyleRegistry& styles() { return styles_; }
  ActionBus& actions() { return actions_; }
  Bindings& bindings() { return bindings_; }

  // Bind a callback (by action name) to `obj` for event `code` (e.g. CLICKED).
  void attachAction(lv_obj_t* obj, const String& action, lv_event_code_t code);

  // Store / fetch an object by its schema "id" (so C++ can reference it later).
  void registerId(const String& id, lv_obj_t* obj) { ids_[id] = obj; }
  lv_obj_t* byId(const String& id) {
    auto it = ids_.find(id);
    return it == ids_.end() ? nullptr : it->second;
  }

  // Wired by UiFactory; lets a builder recurse into nested nodes.
  void setNodeBuilder(NodeBuilder b) { nodeBuilder_ = std::move(b); }
  lv_obj_t* buildNode(lv_obj_t* parent, JsonObjectConst node) {
    return nodeBuilder_ ? nodeBuilder_(parent, node) : nullptr;
  }

 private:
  // Bridge from lv_event_cb (C) to ActionBus; kept alive in links_.
  struct ActionLink {
    ActionBus* bus;
    String action;
  };
  static void onEvent(lv_event_t* e);

  ComponentRegistry components_;
  StyleRegistry styles_;
  ActionBus actions_;
  Bindings bindings_;
  NodeBuilder nodeBuilder_;
  std::vector<std::unique_ptr<ActionLink>> links_;
  std::map<String, lv_obj_t*> ids_;
};

}  // namespace ui
