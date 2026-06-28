/**
 * FeatureRegistry.h - Singleton list of all background features. Features self-register
 * at startup via APP_REGISTER_FEATURE (see AppRegister.h); App just begin()s them all.
 */
#pragma once

#include <vector>

#include "IFeature.h"

namespace app {

class FeatureRegistry {
 public:
  static FeatureRegistry& instance() {
    static FeatureRegistry r;
    return r;
  }

  void add(IFeature* feature) { features_.push_back(feature); }

  const std::vector<IFeature*>& all() const { return features_; }

 private:
  std::vector<IFeature*> features_;
};

}  // namespace app
