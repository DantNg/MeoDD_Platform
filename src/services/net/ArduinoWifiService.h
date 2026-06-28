/**
 * ArduinoWifiService.h - IWifiService implemented with the Arduino WiFi library.
 */
#pragma once

#include "IWifiService.h"

namespace net {

class ArduinoWifiService : public IWifiService {
 public:
  int scan(std::vector<WifiNetwork>& out) override;
  bool connect(const String& ssid, const String& pass,
               uint32_t timeoutMs = 15000) override;
  void disconnect() override;
  bool isConnected() const override;
  String ipAddress() const override;
  int rssi() const override;
};

}  // namespace net
