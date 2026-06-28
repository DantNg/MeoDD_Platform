/**
 * IPage.h - A screen in the app. A page owns everything it needs: its bindings and
 * actions (registered once in onRegister) and how it builds its UI (onShow). Adding
 * a screen = add one IPage implementation + APP_REGISTER_PAGE - no edits elsewhere.
 *
 * onShow runs on the GUI task with `root` being a fresh full-screen container; the
 * page typically builds it from a JSON schema via ctx.ui's UiFactory.
 */
#pragma once

#include <lvgl.h>

#include "../AppContext.h"

namespace app {

class IPage {
 public:
  virtual ~IPage() = default;

  // Stable id used for navigation (e.g. "dashboard", "demo").
  virtual const char* id() const = 0;

  // Register page-local bindings/actions/styles. Called once at startup.
  virtual void onRegister(AppContext& /*ctx*/) {}

  // Build the page UI onto `root`. Called on the GUI task on every navigation.
  virtual void onShow(lv_obj_t* root, AppContext& ctx) = 0;

  // Optional cleanup when navigating away (stop page-local timers, etc.).
  virtual void onHide() {}
};

}  // namespace app
