/**
 * NvsKeyValueStore.cpp
 */
#include "NvsKeyValueStore.h"

namespace nvs {

bool NvsKeyValueStore::begin(const char* ns) {
  if (open_) prefs_.end();
  open_ = prefs_.begin(ns, /*readOnly=*/false);
  return open_;
}

bool NvsKeyValueStore::putString(const char* key, const String& value) {
  return prefs_.putString(key, value) > 0 || value.isEmpty();
}

String NvsKeyValueStore::getString(const char* key, const String& def) {
  return prefs_.isKey(key) ? prefs_.getString(key, def) : def;
}

bool NvsKeyValueStore::putInt(const char* key, int32_t value) {
  return prefs_.putInt(key, value) > 0;
}

int32_t NvsKeyValueStore::getInt(const char* key, int32_t def) {
  return prefs_.isKey(key) ? prefs_.getInt(key, def) : def;
}

bool NvsKeyValueStore::putBool(const char* key, bool value) {
  return prefs_.putBool(key, value) > 0;
}

bool NvsKeyValueStore::getBool(const char* key, bool def) {
  return prefs_.isKey(key) ? prefs_.getBool(key, def) : def;
}

bool NvsKeyValueStore::has(const char* key) { return prefs_.isKey(key); }

bool NvsKeyValueStore::remove(const char* key) { return prefs_.remove(key); }

void NvsKeyValueStore::clear() { prefs_.clear(); }

}  // namespace nvs
