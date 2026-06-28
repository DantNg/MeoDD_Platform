/**
 * ActionBus.h - Maps an action name (a string in the schema, e.g. "nav.settings")
 * to a C++ callback. The schema never names a C++ function; it only refers by
 * name -> decoupled.
 */
#pragma once

#include <Arduino.h>
#include <lvgl.h>

#include <functional>
#include <map>

namespace ui {

class ActionBus {
 public:
  using Handler = std::function<void(lv_event_t*)>;

  void on(const String& action, Handler h) { handlers_[action] = std::move(h); }

  void dispatch(const String& action, lv_event_t* e) {
    auto it = handlers_.find(action);
    if (it != handlers_.end() && it->second) it->second(e);
  }

  bool has(const String& action) const { return handlers_.count(action) > 0; }

 private:
  std::map<String, Handler> handlers_;
};

}  // namespace ui
