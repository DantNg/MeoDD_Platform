/**
 * DashboardPage.cpp - The GPS performance-meter screen. Owns its bindings (symbols +
 * live "race.*" values read from the Store), its background image asset, and its
 * actions. Self-registers - App/main.cpp never mention it.
 */
#include <lvgl.h>

#include "../../assets/images/Images.h"
#include "../../assets/ui/Screens.h"
#include "../../ui/UiContext.h"
#include "../../ui/UiFactory.h"
#include "../AppRegister.h"
#include "../EventBus.h"
#include "../Store.h"
#include "IPage.h"

namespace app {

class DashboardPage : public IPage {
 public:
  const char* id() const override { return "dashboard"; }

  void onRegister(AppContext& ctx) override {
    ctx_ = &ctx;
    auto& b = ctx.ui.bindings();

    // LVGL symbol glyphs used by the schema ("{sym.*}").
    b.provide("sym.gps",      [] { return String(LV_SYMBOL_GPS); });
    b.provide("sym.wifi",     [] { return String(LV_SYMBOL_WIFI); });
    b.provide("sym.settings", [] { return String(LV_SYMBOL_SETTINGS); });
    b.provide("sym.charge",   [] { return String(LV_SYMBOL_CHARGE); });

    // Live telemetry comes from the Store (written by feature tasks).
    Store& store = ctx.store;
    b.provide("race.speed", [&store] { return store.get("race.speed", "0"); });

    // This page owns its background image asset.
    ctx.ui.registerImage("background", &background_img);

    // Actions.
    ctx.ui.actions().on("race.start", [&ctx](lv_event_t*) {
      ctx.bus.publish("race.start");
      Serial.println("[Race] START pressed - launch armed");
    });
  }

  void onShow(lv_obj_t* root, AppContext& ctx) override {
    ui::UiFactory factory(ctx.ui);
    factory.buildFromJson(root, assets::kRaceDashboard);
  }

 private:
  AppContext* ctx_ = nullptr;
};

APP_REGISTER_PAGE(DashboardPage);

}  // namespace app
