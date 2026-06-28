/**
 * IMicrophone.h - Microphone: reads 16-bit mono PCM. Optional (FEATURE_AUDIO_MIC).
 */
#pragma once

#include <Arduino.h>

namespace audio {

class IMicrophone {
 public:
  virtual ~IMicrophone() = default;

  virtual bool begin() = 0;
  // Read up to `maxCount` samples into `out`. Returns the number of samples read.
  virtual size_t read(int16_t* out, size_t maxCount) = 0;
};

}  // namespace audio
