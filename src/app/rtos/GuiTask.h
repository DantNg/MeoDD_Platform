/**
 * GuiTask.h - The one FreeRTOS task that owns LVGL. It initializes the LVGL lock
 * and the GUI dispatch queue, runs an `onReady` callback once on the GUI thread
 * (used to build the first screen), then loops forever pumping LVGL and draining
 * posted jobs - all under the LVGL lock. No other task may call LVGL directly.
 */
#pragma once

#include <cstdint>
#include <functional>

namespace app {
namespace rtos {

struct GuiTaskConfig {
  std::function<void()> onReady;  // runs once on the GUI thread, under the lock
  uint32_t stackBytes = 12 * 1024;
  int priority = 3;
  int coreId = 1;          // pin to APP_CPU; -1 = no affinity
  uint32_t periodMs = 5;   // LVGL service period
};

// Start the GUI task. Call once from setup() after lvgl_port::begin().
void guiTaskStart(const GuiTaskConfig& cfg);

}  // namespace rtos
}  // namespace app
