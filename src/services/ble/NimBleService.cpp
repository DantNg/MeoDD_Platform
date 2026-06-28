/**
 * NimBleService.cpp
 */
#include "../../platform/Features.h"

#if FEATURE_BLE
#include "NimBleService.h"

#include <NimBLEDevice.h>

namespace ble {

bool NimBleService::begin(const String& deviceName) {
  NimBLEDevice::init(deviceName.c_str());
  NimBLEDevice::createServer();
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->start();
  advertising_ = true;
  return true;
}

void NimBleService::stop() {
  NimBLEDevice::stopAdvertising();
  advertising_ = false;
}

}  // namespace ble
#endif  // FEATURE_BLE
