/**
 * AppRegister.h - Self-registration macros. Drop one of these at file scope in a
 * page/feature .cpp and it joins the registry automatically at boot (a static object
 * plus a static bool whose initializer has the side effect of registering it).
 *
 * Files under src/ are linked directly into the firmware (not a gc-sectioned static
 * library), so these initializers always run.
 *
 *   APP_REGISTER_PAGE(DashboardPage);
 *   APP_REGISTER_FEATURE(SysMonFeature);
 */
#pragma once

#include "features/FeatureRegistry.h"
#include "pages/PageRegistry.h"

#define APP_REGISTER_PAGE(T)                          \
  static T s_##T##_obj;                                \
  static const bool s_##T##_reg =                      \
      (::app::PageRegistry::instance().add(&s_##T##_obj), true)

#define APP_REGISTER_FEATURE(T)                        \
  static T s_##T##_obj;                                \
  static const bool s_##T##_reg =                      \
      (::app::FeatureRegistry::instance().add(&s_##T##_obj), true)
