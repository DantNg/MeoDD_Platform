/**
 * IOtaService.h - Over-the-air firmware update. Pure interface -> always compiles;
 * the heavy implementation (HTTPClient/Update) is only pulled in when FEATURE_OTA.
 */
#pragma once

#include <Arduino.h>

namespace ota {

class IOtaService {
 public:
  virtual ~IOtaService() = default;

  virtual bool begin() = 0;

  // Download & flash firmware from a URL (HTTP). True on success (caller should
  // reboot).
  virtual bool updateFromUrl(const String& url) = 0;

  virtual const String& lastMessage() const = 0;
};

}  // namespace ota
