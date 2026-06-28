/**
 * IBoard.h - Hardware facade for ONE board: display + touch + metadata.
 *
 * This is the platform's main extension point: add a new board = write a class
 * implementing IBoard under boards/<name>/ and register one branch in
 * BoardRegistry.h. LvglPort, services and the UI all depend on
 * IBoard/IDisplay/ITouch (interfaces), never on a concrete board (Dependency
 * Inversion).
 */
#pragma once

#include "IDisplay.h"
#include "ITouch.h"

namespace hal {

class IBoard {
 public:
  virtual ~IBoard() = default;

  virtual const char* name() const = 0;

  // Initialize display + touch. Return false if the display failed.
  virtual bool begin() = 0;

  virtual IDisplay& display() = 0;
  virtual ITouch& touch() = 0;
};

}  // namespace hal
