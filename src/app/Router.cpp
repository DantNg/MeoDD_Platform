/**
 * Router.cpp - Navigation implementation.
 */
#include "Router.h"

#include <lvgl.h>

#include "../ui/UiContext.h"
#include "AppContext.h"
#include "pages/IPage.h"
#include "pages/PageRegistry.h"

namespace app {

void Router::init(AppContext& ctx) {
  ctx_ = &ctx;

  // "nav.go:<id>" - jump to a page; arg is the target id.
  ctx.ui.actions().onArg("nav.go", [this](lv_event_t*, const String& id) {
    show(id);
  });
  // "nav.back" - return to the previous page.
  ctx.ui.actions().on("nav.back", [this](lv_event_t*) { back(); });
}

void Router::show(const String& id) { navigate(id, true); }

void Router::back() {
  if (history_.empty()) return;
  const String id = history_.back();
  history_.pop_back();
  navigate(id, false);
}

void Router::navigate(const String& id, bool pushHistory) {
  if (!ctx_) return;
  IPage* page = PageRegistry::instance().find(id);
  if (!page) {
    Serial.printf("[Router] unknown page: '%s'\n", id.c_str());
    return;
  }
  if (pushHistory && current_ && currentId_ != id)
    history_.push_back(currentId_);

  if (current_) current_->onHide();

  // Drop stale widget references before tearing the old tree down.
  ctx_->ui.reset();
  lv_obj_t* scr = lv_scr_act();
  lv_obj_clean(scr);

  current_ = page;
  currentId_ = id;
  page->onShow(scr, *ctx_);
}

}  // namespace app
