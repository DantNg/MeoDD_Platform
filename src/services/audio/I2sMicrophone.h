/**
 * I2sMicrophone.h - IMicrophone over I2S. SKELETON. Only compiled when FEATURE_AUDIO_MIC.
 */
#pragma once

#include "../../platform/Features.h"

#if FEATURE_AUDIO_MIC
#include "IMicrophone.h"

namespace audio {

class I2sMicrophone : public IMicrophone {
 public:
  I2sMicrophone(int bclk, int lrc, int din) : bclk_(bclk), lrc_(lrc), din_(din) {}

  bool begin() override;
  size_t read(int16_t* out, size_t maxCount) override;

 private:
  int bclk_, lrc_, din_;
  bool ready_ = false;
};

}  // namespace audio
#endif  // FEATURE_AUDIO_MIC
