/**
 * I2sSpeaker.h - ISpeaker over I2S. SKELETON: pins configured via the constructor,
 * I2S init/write left as TODO. Only compiled when FEATURE_AUDIO_SPEAKER.
 */
#pragma once

#include "../../platform/Features.h"

#if FEATURE_AUDIO_SPEAKER
#include "ISpeaker.h"

namespace audio {

class I2sSpeaker : public ISpeaker {
 public:
  I2sSpeaker(int bclk, int lrc, int dout) : bclk_(bclk), lrc_(lrc), dout_(dout) {}

  bool begin() override;
  size_t play(const int16_t* samples, size_t count) override;
  void setVolume(uint8_t vol0to100) override { vol_ = vol0to100; }

 private:
  int bclk_, lrc_, dout_;
  uint8_t vol_ = 80;
  bool ready_ = false;
};

}  // namespace audio
#endif  // FEATURE_AUDIO_SPEAKER
