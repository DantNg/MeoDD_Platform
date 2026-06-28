/**
 * I2sMicrophone.cpp - SKELETON. Fill in the real I2S code at the TODOs when needed.
 */
#include "../../platform/Features.h"

#if FEATURE_AUDIO_MIC
#include "I2sMicrophone.h"

namespace audio {

bool I2sMicrophone::begin() {
  // TODO: configure I2S in RX mode with bclk_/lrc_/din_.
  ready_ = true;
  return ready_;
}

size_t I2sMicrophone::read(int16_t* out, size_t maxCount) {
  if (!ready_ || out == nullptr) return 0;
  // TODO: i2s_channel_read(...).
  return 0;
}

}  // namespace audio
#endif  // FEATURE_AUDIO_MIC
