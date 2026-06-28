/**
 * SpinnerBuilder.h - "spinner": indeterminate loading indicator.
 * "period" (ms per turn) and "arc" (arc length in degrees).
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class SpinnerBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node, UiContext&) override {
    return lv_spinner_create(parent, node["period"] | 1000, node["arc"] | 60);
  }
};

}  // namespace ui
