/**
 * IBleService.h - Minimal BLE service (advertising). Pure interface -> always
 * compiles; the NimBLE implementation is only pulled in when FEATURE_BLE (heavy
 * library).
 */
#pragma once

#include <Arduino.h>

namespace ble {

class IBleService {
 public:
  virtual ~IBleService() = default;

  virtual bool begin(const String& deviceName) = 0;
  virtual void stop() = 0;
  virtual bool isAdvertising() const = 0;
};

}  // namespace ble
