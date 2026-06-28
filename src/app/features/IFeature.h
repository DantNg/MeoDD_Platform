/**
 * IFeature.h - A background module (GPS, sensors, telemetry, ...). begin() typically
 * spawns its own FreeRTOS task and publishes results into the Store / EventBus; it
 * must never touch LVGL directly (hop via rtos::guiPost if needed). Adding a feature
 * = add one IFeature implementation + APP_REGISTER_FEATURE - no edits elsewhere.
 */
#pragma once

#include "../AppContext.h"

namespace app {

class IFeature {
 public:
  virtual ~IFeature() = default;

  // Stable id for logging/debug (e.g. "sysmon", "gps").
  virtual const char* id() const = 0;

  // Start the feature (create task, subscribe to events). Called once at startup.
  virtual void begin(AppContext& ctx) = 0;
};

}  // namespace app
