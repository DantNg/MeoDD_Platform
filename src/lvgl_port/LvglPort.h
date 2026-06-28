/**
 * LvglPort.h - The ONE bridge between the HAL and LVGL.
 * Takes an IBoard, registers display (flush) + touch (read) into LVGL, and
 * allocates the draw buffer (PSRAM if available, internal RAM otherwise). The
 * rest of the app works with pure LVGL APIs -> fully board-agnostic.
 */
#pragma once

#include <stdint.h>

#include "../hal/IBoard.h"

namespace lvgl_port {

// Call once in setup(): init board + LVGL + drivers. False if buffer alloc fails.
bool begin(hal::IBoard& board);

// Call periodically in loop(): pumps lv_timer_handler().
void pump();

// Set the backlight 0..255.
void setBacklight(uint8_t value);

}  // namespace lvgl_port
