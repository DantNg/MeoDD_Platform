/**
 * PageRegistry.h - Singleton list of all pages. Pages self-register at startup via
 * APP_REGISTER_PAGE (see AppRegister.h), so neither App nor main.cpp references any
 * concrete page - the registry is the only coupling point (Open/Closed).
 */
#pragma once

#include <Arduino.h>

#include <vector>

#include "IPage.h"

namespace app {

class PageRegistry {
 public:
  static PageRegistry& instance() {
    static PageRegistry r;
    return r;
  }

  void add(IPage* page) { pages_.push_back(page); }

  IPage* find(const String& id) const {
    for (IPage* p : pages_)
      if (id == p->id()) return p;
    return nullptr;
  }

  const std::vector<IPage*>& all() const { return pages_; }

 private:
  std::vector<IPage*> pages_;
};

}  // namespace app
