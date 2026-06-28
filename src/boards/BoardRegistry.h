/**
 * BoardRegistry.h - Selects the ACTIVE board via a BOARD_* macro (set in
 * platformio.ini).
 *
 * This is just a registry: add a board = add one #elif branch pointing at the new
 * IBoard class. There is no hardware logic here, so the platform stays Open/Closed
 * (LvglPort / services / UI are untouched).
 */
#pragma once

#if defined(BOARD_CROWPANEL_S3)
#include "crowpanel_s3/CrowPanelS3Board.h"
namespace boards {
using ActiveBoard = CrowPanelS3Board;
}
#elif defined(BOARD_ESP32C3_ILI9341)
#include "esp32c3_ili9341/Esp32C3Ili9341Board.h"
namespace boards {
using ActiveBoard = Esp32C3Ili9341Board;
}
#else
#error "No BOARD_* defined. Add -D BOARD_xxx to build_flags in platformio.ini"
#endif
