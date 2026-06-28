/**
 * GuiTask.cpp - Implementation of the LVGL-owning task.
 */
#include "GuiTask.h"

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../../lvgl_port/LvglPort.h"
#include "GuiDispatch.h"
#include "LvglLock.h"

namespace app {
namespace rtos {
namespace {

GuiTaskConfig s_cfg;

void guiTaskFn(void* /*arg*/) {
  // Build the first screen on the GUI thread, under the lock.
  if (s_cfg.onReady) {
    LvglGuard guard;
    s_cfg.onReady();
  }

  const TickType_t period = pdMS_TO_TICKS(s_cfg.periodMs);
  for (;;) {
    {
      LvglGuard guard;
      lvgl_port::pump();      // lv_timer_handler()
      guiDispatchDrain();     // run jobs posted by worker tasks
    }
    vTaskDelay(period);
  }
}

}  // namespace

void guiTaskStart(const GuiTaskConfig& cfg) {
  s_cfg = cfg;
  lvglLockInit();
  guiDispatchInit();

  // Clamp the requested core to what the chip actually has (C3 is single-core).
  const bool validCore = cfg.coreId >= 0 && cfg.coreId < portNUM_PROCESSORS;
  const BaseType_t core = validCore ? cfg.coreId : tskNO_AFFINITY;
  xTaskCreatePinnedToCore(guiTaskFn, "gui", cfg.stackBytes / sizeof(StackType_t),
                          nullptr, cfg.priority, nullptr, core);
}

}  // namespace rtos
}  // namespace app
