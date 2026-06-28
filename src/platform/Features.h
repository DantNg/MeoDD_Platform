/**
 * Features.h - CENTRAL feature-toggle table for the platform. Defaults live here;
 * override per-board with -D FEATURE_X=0/1 in platformio.ini.
 *
 * Convention: each service in PlatformBuilder is wrapped in #if FEATURE_*. So
 * removing a feature = turn the macro off -> the code is not compiled and costs
 * no flash.
 */
#pragma once

// --- Storage / config ---
#ifndef FEATURE_SD
#define FEATURE_SD 1            // SD card (SPI)
#endif
#ifndef FEATURE_NVS
#define FEATURE_NVS 1           // persistent key-value in NVS (Preferences)
#endif

// --- Connectivity ---
#ifndef FEATURE_WIFI
#define FEATURE_WIFI 1
#endif
#ifndef FEATURE_OTA
#define FEATURE_OTA 0           // needs a 2 app-slot partition -> off by default
#endif
#ifndef FEATURE_BLE
#define FEATURE_BLE 0           // NimBLE -> enable only when needed (flash cost)
#endif

// --- Audio (optional) ---
#ifndef FEATURE_AUDIO_SPEAKER
#define FEATURE_AUDIO_SPEAKER 0 // speaker over I2S
#endif
#ifndef FEATURE_AUDIO_MIC
#define FEATURE_AUDIO_MIC 0     // microphone over I2S
#endif

// --- System / UI ---
#ifndef FEATURE_SYSTEM_INFO
#define FEATURE_SYSTEM_INFO 1   // chip/heap info for UI bindings
#endif
#ifndef FEATURE_UI_FROM_SD
#define FEATURE_UI_FROM_SD 0    // load JSON schema from the SD card instead of PROGMEM
#endif

// --- External-library drivers ---
// Enable when an env adds the matching library in lib_deps; off -> the driver
// .cpp compiles empty (avoids missing-header errors on boards that don't use it).
#ifndef DRIVER_GT911
#define DRIVER_GT911 0          // GT911 touch (TAMC_GT911) - enabled on CrowPanel-S3
#endif
