/**
 * Router.h - Page navigation. Resolves a page id from the PageRegistry, clears the
 * screen, and asks the page to rebuild its UI. Registers the built-in actions
 * "nav.go:<id>" and "nav.back" so any JSON schema can navigate without new C++.
 *
 * All methods run on the GUI task (called from onReady or from LVGL event handlers),
 * which already holds the LVGL lock.
 */
#pragma once

#include <Arduino.h>

#include <vector>

namespace app {

struct AppContext;
class IPage;

class Router {
 public:
  // Wire the context and register the nav.* actions. Call once at startup.
  void init(AppContext& ctx);

  // Navigate to a page id (pushes the current page onto the back stack).
  void show(const String& id);

  // Return to the previous page, if any.
  void back();

  const String& currentId() const { return currentId_; }

 private:
  void navigate(const String& id, bool pushHistory);

  AppContext* ctx_ = nullptr;
  IPage* current_ = nullptr;
  String currentId_;
  std::vector<String> history_;
};

}  // namespace app
