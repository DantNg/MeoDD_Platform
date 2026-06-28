/**
 * PlatformBuilder.h - Assembles a Platform from the board + the enabled services.
 *
 * This is the ONE place that chooses a concrete implementation for each interface
 * (DIP). Toggle a feature = change a FEATURE_* macro in Features.h or
 * platformio.ini, no edits elsewhere.
 */
#pragma once

#include "../hal/IBoard.h"
#include "Platform.h"

namespace platform {

class PlatformBuilder {
 public:
  // Returns a reference to a static Platform (lives for the whole program).
  static Platform& build(hal::IBoard& board);
};

}  // namespace platform
