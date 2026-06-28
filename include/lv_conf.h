/**
 * lv_conf.h - LVGL 8.3.6 configuration for the multi-board ESP32 LVGL platform.
 * Enabled via the build flag: -D LV_CONF_INCLUDE_SIMPLE
 */
#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================  COLOR / DISPLAY  ====================*/
#define LV_COLOR_DEPTH      16
/* RGB panel + LovyanGFX: we swap bytes ourselves via setSwapBytes(true) in flush,
   so let LVGL not swap. If colors look wrong (e.g. red<->blue) flip this value. */
#define LV_COLOR_16_SWAP    0

/*====================  MEMORY  ====================*/
/* LVGL's internal heap lives in SRAM; the draw buffer & framebuffer live in PSRAM
   (allocated separately in LvglPort.cpp). */
#define LV_MEM_CUSTOM       0
#define LV_MEM_SIZE         (48U * 1024U)
#define LV_MEM_ADR          0
#define LV_MEM_BUF_MAX_NUM  16
#define LV_MEMCPY_MEMSET_STD 0

/*====================  HAL / TICK  ====================*/
/* Tick provided by esp_timer (lv_tick_inc) -> custom = 0, we call lv_tick_inc ourselves. */
#define LV_TICK_CUSTOM      0
#define LV_DPI_DEF          130

/*====================  DRAWING  ====================*/
#define LV_DRAW_COMPLEX     1
#define LV_SHADOW_CACHE_SIZE 0
#define LV_CIRCLE_CACHE_SIZE 4
#define LV_IMG_CACHE_DEF_SIZE 0
#define LV_GRADIENT_MAX_STOPS 2
#define LV_DISP_DEF_REFR_PERIOD 16   /* ~60 FPS */
#define LV_INDEV_DEF_READ_PERIOD 20

/*====================  OS / LOG  ====================*/
#define LV_USE_OS           0
#define LV_USE_LOG          0
#define LV_USE_ASSERT_NULL  1
#define LV_USE_ASSERT_MALLOC 1
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ   0

/*====================  FONT  ====================*/
/* Montserrat for UI text; UNSCII is a full monospace pixel font. */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_UNSCII_8    1
#define LV_FONT_UNSCII_16   1
#define LV_FONT_DEFAULT     &lv_font_montserrat_14
#define LV_FONT_FMT_TXT_LARGE 0
#define LV_USE_FONT_COMPRESSED 0
#define LV_USE_FONT_PLACEHOLDER 1

/*====================  TEXT  ====================*/
#define LV_TXT_ENC          LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS  " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_USE_BIDI         0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*====================  WIDGETS  ====================*/
#define LV_USE_ARC          1
#define LV_USE_BAR          1
#define LV_USE_BTN          1
#define LV_USE_BTNMATRIX    1
#define LV_USE_CANVAS       1
#define LV_USE_CHECKBOX     1
#define LV_USE_DROPDOWN     1
#define LV_USE_IMG          1
#define LV_USE_LABEL        1
#define LV_LABEL_TEXT_SELECTION 1
#define LV_LABEL_LONG_TXT_HINT 1
#define LV_USE_LINE         1
#define LV_USE_ROLLER       1
#define LV_USE_SLIDER       1
#define LV_USE_SWITCH       1
#define LV_USE_TEXTAREA     1
#define LV_USE_TABLE        1
#define LV_USE_LED          1

/*====================  EXTRA / LAYOUT  ====================*/
#define LV_USE_FLEX         1
#define LV_USE_GRID         1
#define LV_USE_TABVIEW      1
#define LV_USE_TILEVIEW     1
#define LV_USE_LIST         1
#define LV_USE_MENU         1
#define LV_USE_MSGBOX       1
#define LV_USE_SPINBOX      1
#define LV_USE_SPINNER      1
#define LV_USE_KEYBOARD     1
#define LV_USE_WIN          1
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 1   /* dark theme */
/* Disable the grow + transition effect on button press: on the 800x480 RGB panel
   these animations redraw constantly -> typing on the keyboard stutters. Off = smoother. */
#define LV_THEME_DEFAULT_GROW 0
#define LV_THEME_DEFAULT_TRANSITION_TIME 0
#define LV_USE_THEME_BASIC  1
#define LV_USE_THEME_MONO   0

/*====================  MISC  ====================*/
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0
#define LV_USE_REFR_DEBUG   0
#define LV_USE_USER_DATA    1
#define LV_USE_SNAPSHOT     0

#endif /* LV_CONF_H */
