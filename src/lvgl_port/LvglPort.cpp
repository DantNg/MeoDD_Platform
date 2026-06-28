/**
 * LvglPort.cpp - Implementation of the HAL <-> LVGL bridge (not tied to any board).
 */
#include "LvglPort.h"

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <lvgl.h>

namespace lvgl_port {
namespace {

hal::IBoard* s_board = nullptr;
lv_disp_draw_buf_t s_draw_buf;
lv_disp_drv_t s_disp_drv;
lv_indev_drv_t s_indev_drv;
esp_timer_handle_t s_tick_timer = nullptr;

// LVGL needs elapsed time -> esp_timer fires every 1ms.
void tick_cb(void* /*arg*/) { lv_tick_inc(1); }

// Push the rendered area to the screen through IDisplay.
void flush_cb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* px) {
  const int16_t w = area->x2 - area->x1 + 1;
  const int16_t h = area->y2 - area->y1 + 1;
  s_board->display().flush(area->x1, area->y1, w, h,
                           reinterpret_cast<const uint16_t*>(px));
  lv_disp_flush_ready(drv);
}

// Read touch through ITouch.
void touch_read_cb(lv_indev_drv_t* /*drv*/, lv_indev_data_t* data) {
  hal::TouchPoint p;
  if (s_board->touch().read(p) && p.pressed) {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = p.x;
    data->point.y = p.y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

}  // namespace

bool begin(hal::IBoard& board) {
  s_board = &board;
  board.begin();  // init display + touch (a failed display -> blank, still runs)

  lv_init();

  const int16_t w = board.display().width();
  const int16_t h = board.display().height();

  // Draw buffer: large/smooth in PSRAM when present; otherwise small in internal RAM.
  size_t buf_px;
  uint32_t caps;
#if defined(BOARD_HAS_PSRAM)
  buf_px = (size_t)w * (h / 4);  // a quarter screen per buffer
  caps = MALLOC_CAP_SPIRAM;
#else
  buf_px = (size_t)w * 40;  // 40 lines per buffer on chips without PSRAM
  caps = MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL;
#endif
  lv_color_t* buf1 =
      (lv_color_t*)heap_caps_malloc(buf_px * sizeof(lv_color_t), caps);
  lv_color_t* buf2 =
      (lv_color_t*)heap_caps_malloc(buf_px * sizeof(lv_color_t), caps);
  if (!buf1 || !buf2) return false;
  lv_disp_draw_buf_init(&s_draw_buf, buf1, buf2, buf_px);

  lv_disp_drv_init(&s_disp_drv);
  s_disp_drv.hor_res = w;
  s_disp_drv.ver_res = h;
  s_disp_drv.flush_cb = flush_cb;
  s_disp_drv.draw_buf = &s_draw_buf;
  lv_disp_drv_register(&s_disp_drv);

  lv_indev_drv_init(&s_indev_drv);
  s_indev_drv.type = LV_INDEV_TYPE_POINTER;
  s_indev_drv.read_cb = touch_read_cb;
  lv_indev_drv_register(&s_indev_drv);

  const esp_timer_create_args_t targs = {
      .callback = &tick_cb,
      .arg = nullptr,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "lv_tick",
      .skip_unhandled_events = true,
  };
  esp_timer_create(&targs, &s_tick_timer);
  esp_timer_start_periodic(s_tick_timer, 1000);  // 1ms

  return true;
}

void pump() { lv_timer_handler(); }

void setBacklight(uint8_t value) {
  if (s_board) s_board->display().setBacklight(value);
}

}  // namespace lvgl_port
