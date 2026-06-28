/**
 * SdCard.h - SD card over SPI, implements ISdStorage.
 * SPI pins are passed via the constructor (default = CrowPanel 5.0 pins) -> a
 * different board just passes different pins, no need to edit this class.
 */
#pragma once

#include <SD.h>
#include <SPI.h>

#include "ISdStorage.h"

namespace storage {

class SdCard : public ISdStorage {
 public:
  // Default: CrowPanel 5.0 SD pins (CS=10, MOSI=11, CLK=12, MISO=13).
  explicit SdCard(int cs = 10, int mosi = 11, int miso = 13, int clk = 12,
                  uint32_t freqHz = 20000000)
      : cs_(cs), mosi_(mosi), miso_(miso), clk_(clk), freq_(freqHz) {}

  bool begin() override;
  bool mounted() const override { return mounted_; }
  fs::FS& fs() override { return SD; }

 private:
  int cs_, mosi_, miso_, clk_;
  uint32_t freq_;
  bool mounted_ = false;
  SPIClass spi_{HSPI};
};

}  // namespace storage
