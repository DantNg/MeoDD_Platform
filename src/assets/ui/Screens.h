/**
 * Screens.h - JSON schema of the demo UI, embedded in flash (PROGMEM-style const).
 * With FEATURE_UI_FROM_SD=1 the same JSON could be loaded from /ui/*.json instead.
 */
#pragma once

namespace assets {

// A full feature-demo screen: a tabview with Dashboard / Network / Storage /
// Controls / System / About tabs.
extern const char kDemoScreen[];

// A GPS drag-strip performance meter (0-60, 1/8 & 1/4 mile, top speed, HP).
extern const char kRaceDashboard[];

}  // namespace assets
