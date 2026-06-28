/**
 * IKeyValueStore.h - Persistent key-value storage (NVS). Generalized from the old
 * credential store -> usable for any config (wifi, ui, app...).
 */
#pragma once

#include <Arduino.h>

namespace nvs {

class IKeyValueStore {
 public:
  virtual ~IKeyValueStore() = default;

  // Open a namespace (key space). Call before reading/writing.
  virtual bool begin(const char* ns) = 0;

  virtual bool putString(const char* key, const String& value) = 0;
  virtual String getString(const char* key, const String& def = String()) = 0;

  virtual bool putInt(const char* key, int32_t value) = 0;
  virtual int32_t getInt(const char* key, int32_t def = 0) = 0;

  virtual bool putBool(const char* key, bool value) = 0;
  virtual bool getBool(const char* key, bool def = false) = 0;

  virtual bool has(const char* key) = 0;
  virtual bool remove(const char* key) = 0;
  virtual void clear() = 0;  // erase the whole open namespace
};

}  // namespace nvs
