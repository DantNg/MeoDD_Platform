/**
 * ArduinoOtaService.h - OTA over HTTP using the Arduino-ESP32 Update library.
 * The whole class only exists when FEATURE_OTA is on (avoids flash cost when off).
 */
#pragma once

#include "../../platform/Features.h"

#if FEATURE_OTA
#include "IOtaService.h"

namespace ota {

class ArduinoOtaService : public IOtaService {
 public:
  bool begin() override;
  bool updateFromUrl(const String& url) override;
  const String& lastMessage() const override { return msg_; }

 private:
  String msg_;
};

}  // namespace ota
#endif  // FEATURE_OTA
