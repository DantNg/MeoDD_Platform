/**
 * ISdStorage.h - SD card abstraction. UI/app depend on this interface, not on a
 * concrete SD class (DIP) -> easy to fake in tests.
 */
#pragma once

#include <FS.h>

namespace storage {

class ISdStorage {
 public:
  virtual ~ISdStorage() = default;

  // Mount the card. Safe to call multiple times. True if ready.
  virtual bool begin() = 0;
  virtual bool mounted() const = 0;

  // Filesystem for file I/O. Only use when mounted().
  virtual fs::FS& fs() = 0;
};

}  // namespace storage
