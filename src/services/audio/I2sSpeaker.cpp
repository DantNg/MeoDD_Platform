/**
 * I2sSpeaker.cpp - SKELETON. Fill in the real I2S code at the TODOs when needed.
 */
#include "../../platform/Features.h"

#if FEATURE_AUDIO_SPEAKER
#include "I2sSpeaker.h"

namespace audio {

bool I2sSpeaker::begin() {
  // TODO: configure I2S (i2s_new_channel / i2s_channel_init_std...) with bclk_/lrc_/dout_.
  ready_ = true;
  return ready_;
}

size_t I2sSpeaker::play(const int16_t* samples, size_t count) {
  if (!ready_ || samples == nullptr) return 0;
  // TODO: i2s_channel_write(...) (optionally scaled by vol_).
  return 0;
}

}  // namespace audio
#endif  // FEATURE_AUDIO_SPEAKER
