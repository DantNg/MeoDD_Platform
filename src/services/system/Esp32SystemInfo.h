/**
 * Esp32SystemInfo.h - ISystemInfo implemented with the ESP32/Arduino APIs.
 */
#pragma once

#include "ISystemInfo.h"

namespace sys {

class Esp32SystemInfo : public ISystemInfo {
 public:
  String chipModel() const override;
  uint8_t chipCores() const override;
  uint32_t cpuFreqMhz() const override;
  uint32_t flashSizeBytes() const override;
  uint32_t psramSizeBytes() const override;
  uint32_t freePsramBytes() const override;
  uint32_t freeHeapBytes() const override;
  String bootPartition() const override;
  String sdkVersion() const override;
  String macAddress() const override;
};

}  // namespace sys
