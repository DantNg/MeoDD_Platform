/**
 * Images.h - Declarations for image assets embedded in flash as LVGL image
 * descriptors (generated C arrays). Register them with the UiContext so the
 * JSON schema can reference them by name (e.g. "bg_img": "background").
 */
#pragma once

#include <lvgl.h>

extern "C" {
extern const lv_img_dsc_t background_img;
}
