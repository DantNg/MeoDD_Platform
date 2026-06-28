/**
 * Platform.h - Holder for the platform's services (board + enabled services).
 *
 * Accessors return an interface pointer; nullptr if that feature is off. Code
 * above (UI/app) checks for nullptr before use -> toggling a feature never breaks
 * the build. The header only forward-declares interfaces -> no heavy includes.
 */
#pragma once

#include "../hal/IBoard.h"

namespace net { class IWifiService; }
namespace storage { class ISdStorage; }
namespace nvs { class IKeyValueStore; }
namespace sys { class ISystemInfo; }
namespace ota { class IOtaService; }
namespace ble { class IBleService; }
namespace audio { class ISpeaker; class IMicrophone; }

namespace platform {

class Platform {
 public:
  hal::IBoard& board() { return *board_; }

  // Return nullptr if the matching feature is disabled.
  net::IWifiService*   wifi()       { return wifi_; }
  storage::ISdStorage* sd()         { return sd_; }
  nvs::IKeyValueStore* nvs()        { return nvs_; }
  sys::ISystemInfo*    systemInfo() { return sysinfo_; }
  ota::IOtaService*    ota()        { return ota_; }
  ble::IBleService*    ble()        { return ble_; }
  audio::ISpeaker*     speaker()    { return speaker_; }
  audio::IMicrophone*  mic()        { return mic_; }

  // Initialize enabled services (mount SD, begin OTA/BLE/audio...). Call after LVGL.
  void beginServices();

  // --- setters: only PlatformBuilder calls these ---
  void setBoard(hal::IBoard& b)            { board_ = &b; }
  void setWifi(net::IWifiService* s)       { wifi_ = s; }
  void setSd(storage::ISdStorage* s)       { sd_ = s; }
  void setNvs(nvs::IKeyValueStore* s)      { nvs_ = s; }
  void setSystemInfo(sys::ISystemInfo* s)  { sysinfo_ = s; }
  void setOta(ota::IOtaService* s)         { ota_ = s; }
  void setBle(ble::IBleService* s)         { ble_ = s; }
  void setSpeaker(audio::ISpeaker* s)      { speaker_ = s; }
  void setMic(audio::IMicrophone* s)       { mic_ = s; }

 private:
  hal::IBoard* board_ = nullptr;
  net::IWifiService* wifi_ = nullptr;
  storage::ISdStorage* sd_ = nullptr;
  nvs::IKeyValueStore* nvs_ = nullptr;
  sys::ISystemInfo* sysinfo_ = nullptr;
  ota::IOtaService* ota_ = nullptr;
  ble::IBleService* ble_ = nullptr;
  audio::ISpeaker* speaker_ = nullptr;
  audio::IMicrophone* mic_ = nullptr;
};

}  // namespace platform
