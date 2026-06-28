/**
 * ArduinoOtaService.cpp
 */
#include "../../platform/Features.h"

#if FEATURE_OTA
#include "ArduinoOtaService.h"

#include <HTTPClient.h>
#include <Update.h>
#include <WiFi.h>

namespace ota {

bool ArduinoOtaService::begin() {
  msg_ = "OTA ready";
  return true;
}

bool ArduinoOtaService::updateFromUrl(const String& url) {
  if (WiFi.status() != WL_CONNECTED) {
    msg_ = "Wi-Fi not connected";
    return false;
  }
  HTTPClient http;
  if (!http.begin(url)) {
    msg_ = "Invalid URL";
    return false;
  }
  const int code = http.GET();
  if (code != HTTP_CODE_OK) {
    msg_ = String("HTTP error ") + code;
    http.end();
    return false;
  }
  const int len = http.getSize();
  if (len <= 0 || !Update.begin((size_t)len)) {
    msg_ = "Could not start Update";
    http.end();
    return false;
  }
  WiFiClient* stream = http.getStreamPtr();
  const size_t written = Update.writeStream(*stream);
  http.end();
  if (written != (size_t)len) {
    msg_ = "Incomplete write";
    return false;
  }
  if (!Update.end(true)) {
    msg_ = String("Update error ") + Update.getError();
    return false;
  }
  msg_ = "OTA success - reboot required";
  return true;
}

}  // namespace ota
#endif  // FEATURE_OTA
