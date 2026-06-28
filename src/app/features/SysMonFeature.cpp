/**
 * SysMonFeature.cpp - Example background feature. Runs its own FreeRTOS task that
 * periodically writes telemetry into the Store (free heap, uptime, and a demo speed
 * ramp). It never touches LVGL; the GUI task picks the values up through "live"
 * labels. Template for real features (GPS, sensors): one self-registering file.
 */
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../AppRegister.h"
#include "../Store.h"
#include "IFeature.h"

namespace app {

class SysMonFeature : public IFeature {
 public:
  const char* id() const override { return "sysmon"; }

  void begin(AppContext& ctx) override {
    store_ = &ctx.store;
    xTaskCreatePinnedToCore(&SysMonFeature::taskFn, "sysmon", 4096, this, 1,
                            nullptr, 0 /* PRO_CPU, off the UI core */);
  }

 private:
  static void taskFn(void* arg) {
    auto* self = static_cast<SysMonFeature*>(arg);
    int demoSpeed = 0;
    int dir = 1;
    for (;;) {
      Store& s = *self->store_;
      s.set("sys.heap", (long)(ESP.getFreeHeap() / 1024));
      s.set("sys.uptime", (long)(millis() / 1000));

      // Demo: bounce a fake speed 0..120 so the dashboard's live number moves.
      demoSpeed += dir * 3;
      if (demoSpeed >= 120) { demoSpeed = 120; dir = -1; }
      if (demoSpeed <= 0)   { demoSpeed = 0;   dir = 1; }
      s.set("race.speed", (long)demoSpeed);

      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }

  Store* store_ = nullptr;
};

APP_REGISTER_FEATURE(SysMonFeature);

}  // namespace app
