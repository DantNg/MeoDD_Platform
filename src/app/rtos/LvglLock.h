/**
 * LvglLock.h - The single mutex guarding all LVGL access. LVGL is not thread-safe,
 * so only the GUI task touches it directly; any other task must hold this lock
 * (or, preferably, post work via GuiDispatch). RAII guard included.
 */
#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace app {
namespace rtos {

// Create the (recursive) mutex once, before the GUI task starts.
void lvglLockInit();

// Take / give the LVGL mutex. timeoutMs < 0 => wait forever.
bool lvglLock(int timeoutMs = -1);
void lvglUnlock();

// RAII helper: locks on construction, unlocks on scope exit.
class LvglGuard {
 public:
  explicit LvglGuard(int timeoutMs = -1) : held_(lvglLock(timeoutMs)) {}
  ~LvglGuard() { if (held_) lvglUnlock(); }
  bool held() const { return held_; }
  LvglGuard(const LvglGuard&) = delete;
  LvglGuard& operator=(const LvglGuard&) = delete;

 private:
  bool held_;
};

}  // namespace rtos
}  // namespace app
