/**
 * IWifiService.h - Wi-Fi service interface. The UI depends on this, not on
 * <WiFi.h> directly -> easy to fake in tests and to swap the connection backend
 * later (DIP).
 */
#pragma once

#include <Arduino.h>

#include <vector>

namespace net {

// One Wi-Fi network found during a scan.
struct WifiNetwork {
  String ssid;
  int rssi = 0;
  bool open = false;  // true if the network needs no password
};

class IWifiService {
 public:
  virtual ~IWifiService() = default;

  // Scan nearby networks (blocking). Returns the count, fills `out` (sorted by
  // signal strength, no duplicate SSIDs).
  virtual int scan(std::vector<WifiNetwork>& out) = 0;

  // Connect (blocking, with timeout). Returns true on success.
  virtual bool connect(const String& ssid, const String& pass,
                       uint32_t timeoutMs = 15000) = 0;
  virtual void disconnect() = 0;
  virtual bool isConnected() const = 0;
  virtual String ipAddress() const = 0;
  virtual int rssi() const = 0;
};

}  // namespace net
