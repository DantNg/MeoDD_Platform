/**
 * UiContext.cpp - Bridge LVGL events to the ActionBus.
 */
#include "UiContext.h"

namespace ui {

void UiContext::attachAction(lv_obj_t* obj, const String& action,
                             lv_event_code_t code) {
  links_.push_back(
      std::unique_ptr<ActionLink>(new ActionLink{&actions_, action}));
  lv_obj_add_event_cb(obj, &UiContext::onEvent, code, links_.back().get());
}

void UiContext::onEvent(lv_event_t* e) {
  auto* link = static_cast<ActionLink*>(lv_event_get_user_data(e));
  if (link && link->bus) link->bus->dispatch(link->action, e);
}

}  // namespace ui
