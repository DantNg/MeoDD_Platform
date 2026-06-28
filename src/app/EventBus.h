/**
 * EventBus.h - Minimal thread-safe publish/subscribe by topic. Decouples features
 * from pages and from each other: a feature publishes "gps.fix"; whoever cares
 * subscribes by topic, without either side knowing the other.
 *
 * Handlers run on the publisher's task. If a handler needs to touch LVGL, it must
 * hop to the GUI thread via rtos::guiPost(...).
 */
#pragma once

#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <functional>
#include <map>
#include <vector>

namespace app {

class EventBus {
 public:
  using Handler = std::function<void(const String& payload)>;

  EventBus() { mutex_ = xSemaphoreCreateMutex(); }

  void subscribe(const String& topic, Handler h) {
    lock();
    subs_[topic].push_back(std::move(h));
    unlock();
  }

  void publish(const String& topic, const String& payload = String()) {
    // Copy handlers under the lock, then invoke outside it (avoid re-entrancy deadlock).
    std::vector<Handler> handlers;
    lock();
    auto it = subs_.find(topic);
    if (it != subs_.end()) handlers = it->second;
    unlock();
    for (auto& h : handlers)
      if (h) h(payload);
  }

 private:
  void lock()   { if (mutex_) xSemaphoreTake(mutex_, portMAX_DELAY); }
  void unlock() { if (mutex_) xSemaphoreGive(mutex_); }

  SemaphoreHandle_t mutex_ = nullptr;
  std::map<String, std::vector<Handler>> subs_;
};

}  // namespace app
