/**
 * BuiltinComponents.h - Register the default builders into a ComponentRegistry.
 * To add a new widget: create a builder under builders/ then add one reg.add(...)
 * line here.
 */
#pragma once

#include "ComponentRegistry.h"
#include "builders/ArcBuilder.h"
#include "builders/BarBuilder.h"
#include "builders/ButtonBuilder.h"
#include "builders/CheckboxBuilder.h"
#include "builders/ContainerBuilder.h"
#include "builders/DropdownBuilder.h"
#include "builders/ImageBuilder.h"
#include "builders/KeyboardBuilder.h"
#include "builders/LabelBuilder.h"
#include "builders/LedBuilder.h"
#include "builders/ListBuilder.h"
#include "builders/RollerBuilder.h"
#include "builders/ScreenBuilder.h"
#include "builders/SliderBuilder.h"
#include "builders/SpinnerBuilder.h"
#include "builders/SwitchBuilder.h"
#include "builders/TabviewBuilder.h"
#include "builders/TextareaBuilder.h"

namespace ui {

inline void registerBuiltinComponents(ComponentRegistry& reg) {
  static ScreenBuilder screen;
  static ContainerBuilder container;
  static LabelBuilder label;
  static ButtonBuilder button;
  static SliderBuilder slider;
  static SwitchBuilder sw;
  static BarBuilder bar;
  static ImageBuilder image;
  static CheckboxBuilder checkbox;
  static DropdownBuilder dropdown;
  static RollerBuilder roller;
  static ArcBuilder arc;
  static LedBuilder led;
  static SpinnerBuilder spinner;
  static TextareaBuilder textarea;
  static ListBuilder list;
  static TabviewBuilder tabview;
  static KeyboardBuilder keyboard;

  reg.add("screen", &screen);
  reg.add("container", &container);
  reg.add("panel", &container);
  reg.add("label", &label);
  reg.add("button", &button);
  reg.add("slider", &slider);
  reg.add("switch", &sw);
  reg.add("bar", &bar);
  reg.add("image", &image);
  reg.add("checkbox", &checkbox);
  reg.add("dropdown", &dropdown);
  reg.add("roller", &roller);
  reg.add("arc", &arc);
  reg.add("led", &led);
  reg.add("spinner", &spinner);
  reg.add("textarea", &textarea);
  reg.add("list", &list);
  reg.add("tabview", &tabview);
  reg.add("keyboard", &keyboard);
}

}  // namespace ui
