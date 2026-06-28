/**
 * Screens.h - JSON schema of the demo UI, embedded in flash (PROGMEM-style const).
 * With FEATURE_UI_FROM_SD=1 the same JSON could be loaded from /ui/*.json instead.
 */
#pragma once

namespace assets {

// A full feature-demo screen: a tabview with Dashboard / Network / Storage /
// Controls / System / About tabs.
extern const char kDemoScreen[];

}  // namespace assets
