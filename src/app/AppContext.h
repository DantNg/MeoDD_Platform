/**
 * AppContext.h - One bundle of references passed to every page and feature so they
 * reach the engine without global singletons: the UI engine, the platform services,
 * the shared Store, the EventBus and the Router. Pages/features depend on this struct
 * (and interfaces), never on concrete wiring.
 */
#pragma once

namespace ui { class UiContext; }
namespace platform { class Platform; }

namespace app {

class Store;
class EventBus;
class Router;

struct AppContext {
  ui::UiContext& ui;
  platform::Platform& platform;
  Store& store;
  EventBus& bus;
  Router& router;
};

}  // namespace app
