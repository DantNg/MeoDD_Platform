/**
 * LvglLock.cpp - Recursive-mutex implementation of the LVGL lock.
 */
#include "LvglLock.h"

namespace app {
namespace rtos {
namespace {
SemaphoreHandle_t s_mutex = nullptr;
}  // namespace

void lvglLockInit() {
  if (!s_mutex) s_mutex = xSemaphoreCreateRecursiveMutex();
}

bool lvglLock(int timeoutMs) {
  if (!s_mutex) return true;  // not initialized yet (single-threaded startup)
  const TickType_t ticks =
      timeoutMs < 0 ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
  return xSemaphoreTakeRecursive(s_mutex, ticks) == pdTRUE;
}

void lvglUnlock() {
  if (s_mutex) xSemaphoreGiveRecursive(s_mutex);
}

}  // namespace rtos
}  // namespace app
