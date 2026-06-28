/**
 * ISpeaker.h - Speaker: plays 16-bit mono PCM. Optional (FEATURE_AUDIO_SPEAKER).
 */
#pragma once

#include <Arduino.h>

namespace audio {

class ISpeaker {
 public:
  virtual ~ISpeaker() = default;

  virtual bool begin() = 0;
  // Play `count` 16-bit PCM samples. Returns the number of samples written.
  virtual size_t play(const int16_t* samples, size_t count) = 0;
  virtual void setVolume(uint8_t vol0to100) = 0;
};

}  // namespace audio
