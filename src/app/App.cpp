/**
 * App.cpp - System bring-up and wiring. Knows the engine and the registries, but not
 * any concrete page or feature (those self-register), so adding either needs no edit
 * here.
 */
#include "App.h"

#include <Arduino.h>
#include <lvgl.h>

#include "../lvgl_port/LvglPort.h"
#include "../platform/Platform.h"
#include "../platform/PlatformBuilder.h"
#include "../ui/Theme.h"
#include "../ui/UiContext.h"
#include "../ui/components/BuiltinComponents.h"
#include "AppContext.h"
#include "EventBus.h"
#include "Router.h"
#include "Store.h"
#include "features/FeatureRegistry.h"
#include "pages/PageRegistry.h"
#include "rtos/GuiTask.h"

namespace app {
namespace {

constexpr char kHomePage[] = "dashboard";
constexpr uint32_t kLiveRefreshMs = 200;

// Engine + app singletons: live for the whole program.
ui::UiContext g_ui;
Store g_store;
EventBus g_bus;
Router g_router;

// AppContext references the singletons above; built in run().
AppContext* g_ctx = nullptr;

// GUI-task timer: re-resolve "live" labels from the Store.
void liveRefreshCb(lv_timer_t* /*t*/) { g_ui.refreshLive(); }

}  // namespace

void App::run(hal::IBoard& board) {
  Serial.begin(115200);
  delay(100);
  Serial.printf("\n[App] Board: %s\n", board.name());

  // 1) Hardware + LVGL.
  if (!lvgl_port::begin(board)) {
    Serial.println("[App] ERROR: failed to allocate draw buffer!");
    while (true) delay(1000);
  }
  lvgl_port::setBacklight(255);

  // 2) Platform services (per FEATURE_* macros).
  platform::Platform& platform = platform::PlatformBuilder::build(board);
  platform.beginServices();

  // 3) UI engine: styles + components.
  ui::theme::registerStyles(g_ui.styles());
  ui::registerBuiltinComponents(g_ui.components());

  // 4) Shared context for pages/features.
  static AppContext ctx{g_ui, platform, g_store, g_bus, g_router};
  g_ctx = &ctx;

  // 5) Router + let every page register its bindings/actions/styles.
  g_router.init(ctx);
  for (IPage* p : PageRegistry::instance().all()) p->onRegister(ctx);

  // 6) Start background features (each may spawn its own task).
  for (IFeature* f : FeatureRegistry::instance().all()) {
    Serial.printf("[App] feature: %s\n", f->id());
    f->begin(ctx);
  }

  // 7) Start the GUI task; it builds the home page on the GUI thread.
  rtos::GuiTaskConfig gcfg;
  gcfg.onReady = [] {
    g_router.show(kHomePage);
    lv_timer_create(liveRefreshCb, kLiveRefreshMs, nullptr);
  };
  rtos::guiTaskStart(gcfg);

  Serial.println("[App] Ready.");
}

}  // namespace app
