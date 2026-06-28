/**
 * Esp32SystemInfo.cpp
 */
#include "Esp32SystemInfo.h"

#include <esp_chip_info.h>
#include <esp_mac.h>     // IDF5: esp_read_mac / ESP_MAC_WIFI_STA moved here
#include <esp_ota_ops.h>
#include <esp_system.h>

namespace sys {

String Esp32SystemInfo::chipModel() const { return String(ESP.getChipModel()); }

uint8_t Esp32SystemInfo::chipCores() const { return ESP.getChipCores(); }

uint32_t Esp32SystemInfo::cpuFreqMhz() const { return ESP.getCpuFreqMHz(); }

uint32_t Esp32SystemInfo::flashSizeBytes() const { return ESP.getFlashChipSize(); }

uint32_t Esp32SystemInfo::psramSizeBytes() const { return ESP.getPsramSize(); }

uint32_t Esp32SystemInfo::freePsramBytes() const { return ESP.getFreePsram(); }

uint32_t Esp32SystemInfo::freeHeapBytes() const { return ESP.getFreeHeap(); }

String Esp32SystemInfo::bootPartition() const {
  // Running partition (e.g. "factory"). With OTA this would be ota_0/ota_1.
  const esp_partition_t* p = esp_ota_get_running_partition();
  return p ? String(p->label) : String("unknown");
}

String Esp32SystemInfo::sdkVersion() const { return String(ESP.getSdkVersion()); }

String Esp32SystemInfo::macAddress() const {
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  char buf[18];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2],
           mac[3], mac[4], mac[5]);
  return String(buf);
}

}  // namespace sys
