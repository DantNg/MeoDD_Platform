/**
 * ComponentRegistry.h - Maps "type" (string in the schema) -> IComponentBuilder.
 * Add a widget kind with add(type, builder); UiFactory is untouched.
 */
#pragma once

#include <Arduino.h>

#include <map>

#include "IComponentBuilder.h"

namespace ui {

class ComponentRegistry {
 public:
  void add(const String& type, IComponentBuilder* builder) {
    builders_[type] = builder;
  }

  IComponentBuilder* find(const String& type) {
    auto it = builders_.find(type);
    return it == builders_.end() ? nullptr : it->second;
  }

 private:
  std::map<String, IComponentBuilder*> builders_;
};

}  // namespace ui
