/**
 * NimBleService.h - IBleService using NimBLE-Arduino.
 * NimBLE is only added to lib_deps when FEATURE_BLE=1, so the whole class is
 * wrapped in the macro to keep the default build (FEATURE_BLE=0) library-free.
 */
#pragma once

#include "../../platform/Features.h"

#if FEATURE_BLE
#include "IBleService.h"

namespace ble {

class NimBleService : public IBleService {
 public:
  bool begin(const String& deviceName) override;
  void stop() override;
  bool isAdvertising() const override { return advertising_; }

 private:
  bool advertising_ = false;
};

}  // namespace ble
#endif  // FEATURE_BLE
