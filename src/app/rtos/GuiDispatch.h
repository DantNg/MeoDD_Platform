/**
 * GuiDispatch.h - "Run on the GUI thread". Any task can post a std::function to be
 * executed on the GUI task (while it holds the LVGL lock), which is the safe way to
 * touch LVGL from a worker task. The GUI task calls drain() each cycle.
 */
#pragma once

#include <functional>

namespace app {
namespace rtos {

using GuiJob = std::function<void()>;

// Create the underlying queue once, before the GUI task starts.
void guiDispatchInit();

// Post a job from any task. Returns false if the queue is full.
bool guiPost(GuiJob job);

// Run all pending jobs. Called by the GUI task (already holding the LVGL lock).
void guiDispatchDrain();

}  // namespace rtos
}  // namespace app
