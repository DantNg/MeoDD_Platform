/**
 * App.h - Top-level orchestrator. Replaces the wiring that used to live in main.cpp:
 * bring up LVGL + the platform, build the UI engine, let every page/feature register
 * itself, then start the GUI task on the home page. main.cpp just calls App::run().
 */
#pragma once

#include "../hal/IBoard.h"

namespace app {

class App {
 public:
  // Bring the whole system up. Call once from setup(). Never returns control to a
  // caller that needs the CPU - work proceeds on the GUI task + feature tasks.
  static void run(hal::IBoard& board);
};

}  // namespace app
