/**
 * Store.h - A small thread-safe key->String state holder. Worker tasks write live
 * telemetry here (e.g. "race.speed", "sys.heap"); the GUI task reads it to refresh
 * "live" labels. A global revision counter lets the UI cheaply detect changes.
 *
 * Reads go through the UI Bindings layer: a provider for "race.speed" simply calls
 * Store::get("race.speed"), so the JSON schema stays the single source of layout.
 */
#pragma once

#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <map>

namespace app {

class Store {
 public:
  Store() { mutex_ = xSemaphoreCreateMutex(); }

  void set(const String& key, const String& value) {
    lock();
    auto it = values_.find(key);
    if (it == values_.end() || it->second != value) {
      values_[key] = value;
      ++revision_;
    }
    unlock();
  }

  void set(const String& key, long value) { set(key, String(value)); }

  String get(const String& key, const String& def = String()) {
    lock();
    auto it = values_.find(key);
    String out = (it == values_.end()) ? def : it->second;
    unlock();
    return out;
  }

  // Monotonic counter; increments whenever any value actually changes.
  uint32_t revision() {
    lock();
    uint32_t r = revision_;
    unlock();
    return r;
  }

 private:
  void lock()   { if (mutex_) xSemaphoreTake(mutex_, portMAX_DELAY); }
  void unlock() { if (mutex_) xSemaphoreGive(mutex_); }

  SemaphoreHandle_t mutex_ = nullptr;
  std::map<String, String> values_;
  uint32_t revision_ = 0;
};

}  // namespace app
