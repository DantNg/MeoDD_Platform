/**
 * Platform.cpp - Initialize enabled services. Each branch is wrapped in #if FEATURE_*.
 */
#include "Platform.h"

#include <Arduino.h>

#include "Features.h"

#if FEATURE_SD
#include "../services/storage/ISdStorage.h"
#endif
#if FEATURE_OTA
#include "../services/ota/IOtaService.h"
#endif
#if FEATURE_BLE
#include "../services/ble/IBleService.h"
#endif
#if FEATURE_AUDIO_SPEAKER
#include "../services/audio/ISpeaker.h"
#endif
#if FEATURE_AUDIO_MIC
#include "../services/audio/IMicrophone.h"
#endif

namespace platform {

void Platform::beginServices() {
#if FEATURE_SD
  if (sd_) {
    Serial.println(sd_->begin() ? "[SD] mounted" : "[SD] mount failed");
  }
#endif
#if FEATURE_OTA
  if (ota_) ota_->begin();
#endif
#if FEATURE_BLE
  if (ble_) ble_->begin(board_ ? board_->name() : "ESP32-Platform");
#endif
#if FEATURE_AUDIO_SPEAKER
  if (speaker_) speaker_->begin();
#endif
#if FEATURE_AUDIO_MIC
  if (mic_) mic_->begin();
#endif
}

}  // namespace platform
