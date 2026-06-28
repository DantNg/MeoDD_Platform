/**
 * ISystemInfo.h - Provides system information (chip/heap/...) to the UI bindings.
 * The UI depends on this interface, NOT on ESP-IDF directly (DIP) -> can be faked
 * for PC testing.
 */
#pragma once

#include <Arduino.h>

namespace sys {

class ISystemInfo {
 public:
  virtual ~ISystemInfo() = default;

  virtual String chipModel() const = 0;        // e.g. "ESP32-S3"
  virtual uint8_t chipCores() const = 0;
  virtual uint32_t cpuFreqMhz() const = 0;
  virtual uint32_t flashSizeBytes() const = 0;
  virtual uint32_t psramSizeBytes() const = 0;
  virtual uint32_t freePsramBytes() const = 0;
  virtual uint32_t freeHeapBytes() const = 0;
  virtual String bootPartition() const = 0;     // running partition label
  virtual String sdkVersion() const = 0;
  virtual String macAddress() const = 0;
};

}  // namespace sys
