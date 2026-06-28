/**
 * SdCard.cpp
 */
#include "SdCard.h"

namespace storage {

bool SdCard::begin() {
  if (mounted_) return true;

  spi_.begin(clk_, miso_, mosi_, cs_);
  if (!SD.begin(cs_, spi_, freq_)) {
    return false;
  }
  mounted_ = true;
  return true;
}

}  // namespace storage
