/**
 * PlatformBuilder.cpp - Statically allocate each enabled service and wire it into
 * the Platform.
 */
#include "PlatformBuilder.h"

#include "Features.h"

#if FEATURE_SD
#include "../services/storage/SdCard.h"
#endif
#if FEATURE_NVS
#include "../services/nvs/NvsKeyValueStore.h"
#endif
#if FEATURE_WIFI
#include "../services/net/ArduinoWifiService.h"
#endif
#if FEATURE_SYSTEM_INFO
#include "../services/system/Esp32SystemInfo.h"
#endif
#if FEATURE_OTA
#include "../services/ota/ArduinoOtaService.h"
#endif
#if FEATURE_BLE
#include "../services/ble/NimBleService.h"
#endif
#if FEATURE_AUDIO_SPEAKER
#include "../services/audio/I2sSpeaker.h"
#endif
#if FEATURE_AUDIO_MIC
#include "../services/audio/I2sMicrophone.h"
#endif

namespace platform {

Platform& PlatformBuilder::build(hal::IBoard& board) {
  static Platform s_platform;
  s_platform.setBoard(board);

#if FEATURE_SD
  static storage::SdCard s_sd;
  s_platform.setSd(&s_sd);
#endif
#if FEATURE_NVS
  static nvs::NvsKeyValueStore s_nvs;
  s_platform.setNvs(&s_nvs);
#endif
#if FEATURE_WIFI
  static net::ArduinoWifiService s_wifi;
  s_platform.setWifi(&s_wifi);
#endif
#if FEATURE_SYSTEM_INFO
  static sys::Esp32SystemInfo s_sysinfo;
  s_platform.setSystemInfo(&s_sysinfo);
#endif
#if FEATURE_OTA
  static ota::ArduinoOtaService s_ota;
  s_platform.setOta(&s_ota);
#endif
#if FEATURE_BLE
  static ble::NimBleService s_ble;
  s_platform.setBle(&s_ble);
#endif
#if FEATURE_AUDIO_SPEAKER
  // I2S speaker pins: placeholders, set to the real board pins when used.
  static audio::I2sSpeaker s_speaker(/*bclk=*/0, /*lrc=*/0, /*dout=*/0);
  s_platform.setSpeaker(&s_speaker);
#endif
#if FEATURE_AUDIO_MIC
  static audio::I2sMicrophone s_mic(/*bclk=*/0, /*lrc=*/0, /*din=*/0);
  s_platform.setMic(&s_mic);
#endif

  return s_platform;
}

}  // namespace platform
