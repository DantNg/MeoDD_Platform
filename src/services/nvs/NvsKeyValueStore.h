/**
 * NvsKeyValueStore.h - IKeyValueStore on top of NVS via the Preferences library.
 */
#pragma once

#include <Preferences.h>

#include "IKeyValueStore.h"

namespace nvs {

class NvsKeyValueStore : public IKeyValueStore {
 public:
  bool begin(const char* ns) override;

  bool putString(const char* key, const String& value) override;
  String getString(const char* key, const String& def = String()) override;

  bool putInt(const char* key, int32_t value) override;
  int32_t getInt(const char* key, int32_t def = 0) override;

  bool putBool(const char* key, bool value) override;
  bool getBool(const char* key, bool def = false) override;

  bool has(const char* key) override;
  bool remove(const char* key) override;
  void clear() override;

 private:
  Preferences prefs_;
  bool open_ = false;
};

}  // namespace nvs
