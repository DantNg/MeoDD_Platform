/**
 * ArduinoWifiService.cpp
 */
#include "ArduinoWifiService.h"

#include <WiFi.h>

#include <algorithm>

namespace net {

int ArduinoWifiService::scan(std::vector<WifiNetwork>& out) {
  out.clear();
  WiFi.mode(WIFI_STA);
  WiFi.scanDelete();  // drop old scan results (do NOT disconnect/eraseap -> avoids state corruption)

  const int n = WiFi.scanNetworks(false /*async*/, true /*show_hidden*/);  // blocking ~2-4s
  if (n <= 0) {
    WiFi.scanDelete();
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    WifiNetwork net;
    net.ssid = WiFi.SSID(i);
    net.rssi = WiFi.RSSI(i);
    net.open = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);
    if (net.ssid.isEmpty()) continue;  // skip hidden SSIDs

    // De-duplicate SSIDs, keep the stronger one.
    auto it = std::find_if(out.begin(), out.end(),
                           [&](const WifiNetwork& w) { return w.ssid == net.ssid; });
    if (it == out.end()) {
      out.push_back(net);
    } else if (net.rssi > it->rssi) {
      *it = net;
    }
  }
  WiFi.scanDelete();

  std::sort(out.begin(), out.end(),
            [](const WifiNetwork& a, const WifiNetwork& b) { return a.rssi > b.rssi; });
  return out.size();
}

bool ArduinoWifiService::connect(const String& ssid, const String& pass,
                                 uint32_t timeoutMs) {
  if (isConnected()) return true;

  // Start from a clean state before begin (avoids stale/hung connections after a scan).
  WiFi.persistent(false);  // we manage credentials ourselves via NVS
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false, false);
  delay(100);

  WiFi.begin(ssid.c_str(), pass.c_str());

  // Wait (blocking) up to timeoutMs. Do not bail out on transient errors
  // (CONNECT_FAILED/NO_SSID can be intermediate states before associating).
  const uint32_t start = millis();
  wl_status_t st = WL_IDLE_STATUS;
  while ((millis() - start) < timeoutMs) {
    st = static_cast<wl_status_t>(WiFi.status());
    if (st == WL_CONNECTED) break;
    delay(200);
  }

  Serial.printf("[WiFi] connect '%s' -> status=%d %s\n", ssid.c_str(), (int)st,
                isConnected() ? WiFi.localIP().toString().c_str() : "(failed)");
  return isConnected();
}

void ArduinoWifiService::disconnect() { WiFi.disconnect(true); }

bool ArduinoWifiService::isConnected() const { return WiFi.status() == WL_CONNECTED; }

String ArduinoWifiService::ipAddress() const { return WiFi.localIP().toString(); }

int ArduinoWifiService::rssi() const { return WiFi.RSSI(); }

}  // namespace net
