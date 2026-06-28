/**
 * Bindings.h - Replaces "{key}" tokens in a string (e.g. a label text) with the
 * value from a registered provider (e.g. "sys.heap" -> free heap in KB). Lets the
 * schema show live data.
 */
#pragma once

#include <Arduino.h>

#include <functional>
#include <map>

namespace ui {

class Bindings {
 public:
  using Provider = std::function<String()>;

  void provide(const String& key, Provider p) { providers_[key] = std::move(p); }

  // Return a new string with every {key} replaced; a key without a provider
  // becomes empty.
  String resolve(const String& in) const {
    if (in.indexOf('{') < 0) return in;
    String out;
    out.reserve(in.length() + 16);
    int i = 0;
    const int n = in.length();
    while (i < n) {
      const char c = in[i];
      if (c == '{') {
        const int end = in.indexOf('}', i + 1);
        if (end < 0) {
          out += in.substring(i);
          break;
        }
        const String key = in.substring(i + 1, end);
        auto it = providers_.find(key);
        if (it != providers_.end() && it->second) out += it->second();
        i = end + 1;
      } else {
        out += c;
        i++;
      }
    }
    return out;
  }

 private:
  std::map<String, Provider> providers_;
};

}  // namespace ui
