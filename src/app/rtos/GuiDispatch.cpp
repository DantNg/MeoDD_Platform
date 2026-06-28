/**
 * GuiDispatch.cpp - FreeRTOS-queue implementation of the GUI job dispatcher.
 * The queue holds heap-allocated std::function pointers (move-only friendly).
 */
#include "GuiDispatch.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace app {
namespace rtos {
namespace {
constexpr int kQueueLen = 16;
QueueHandle_t s_queue = nullptr;
}  // namespace

void guiDispatchInit() {
  if (!s_queue) s_queue = xQueueCreate(kQueueLen, sizeof(GuiJob*));
}

bool guiPost(GuiJob job) {
  if (!s_queue) return false;
  GuiJob* p = new GuiJob(std::move(job));
  if (xQueueSend(s_queue, &p, 0) != pdTRUE) {
    delete p;  // queue full: drop the job rather than block the worker
    return false;
  }
  return true;
}

void guiDispatchDrain() {
  if (!s_queue) return;
  GuiJob* p = nullptr;
  while (xQueueReceive(s_queue, &p, 0) == pdTRUE) {
    if (p) {
      (*p)();
      delete p;
    }
  }
}

}  // namespace rtos
}  // namespace app
