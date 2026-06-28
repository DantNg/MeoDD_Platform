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
  // Arg-handler: action written as "name:arg" in the schema (e.g. "nav.go:demo").
  using ArgHandler = std::function<void(lv_event_t*, const String& arg)>;

  void on(const String& action, Handler h) { handlers_[action] = std::move(h); }
  void onArg(const String& action, ArgHandler h) {
    argHandlers_[action] = std::move(h);
  }

  // Dispatch by the full schema string. If it contains "name:arg" and an
  // arg-handler is registered for "name", that wins; otherwise a plain handler
  // for the whole string is used.
  void dispatch(const String& action, lv_event_t* e) {
    const int colon = action.indexOf(':');
    if (colon > 0) {
      const String name = action.substring(0, colon);
      auto ait = argHandlers_.find(name);
      if (ait != argHandlers_.end() && ait->second) {
        ait->second(e, action.substring(colon + 1));
        return;
      }
    }
    auto it = handlers_.find(action);
    if (it != handlers_.end() && it->second) it->second(e);
  }

  bool has(const String& action) const {
    if (handlers_.count(action) > 0) return true;
    const int colon = action.indexOf(':');
    return colon > 0 && argHandlers_.count(action.substring(0, colon)) > 0;
  }

 private:
  std::map<String, Handler> handlers_;
  std::map<String, ArgHandler> argHandlers_;
};

}  // namespace ui
