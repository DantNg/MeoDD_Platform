/**
 * TabviewBuilder.h - "tabview": a tabbed view. Its children are "tab" nodes:
 *   { "type": "tabview", "tab_height": 44, "children": [
 *       { "title": "Home", "children": [ ... ] }, ... ] }
 *
 * This builder consumes its own children (buildsOwnChildren = true): each tab
 * becomes a page, and the tab's children are built into that page via
 * ctx.buildNode(). Pages default to a vertical flex layout.
 *
 * Smoothness on slow (e.g. RGB) panels: the tab-change slide animation is turned
 * off (we snap to the tab with LV_ANIM_OFF after the built-in handler runs), and
 * page scrolling has no momentum/elastic overshoot.
 */
#pragma once

#include "../../UiContext.h"
#include "../IComponentBuilder.h"

namespace ui {

class TabviewBuilder : public IComponentBuilder {
 public:
  lv_obj_t* build(lv_obj_t* parent, JsonObjectConst node,
                  UiContext& ctx) override {
    lv_obj_t* tv = lv_tabview_create(parent, LV_DIR_TOP, node["tab_height"] | 44);

    // Instant tab switching: our handler runs after the tabview's built-in
    // (animated) one and re-selects the tab without animation, cancelling the
    // just-started scroll animation before it renders a frame.
    lv_obj_t* btns = lv_tabview_get_tab_btns(tv);
    lv_obj_add_event_cb(btns, &TabviewBuilder::snapNoAnim, LV_EVENT_VALUE_CHANGED, tv);

    // Disable swipe-to-change-tab: clearing SCROLLABLE on the content container
    // stops the horizontal drag gesture between pages. Tabs still change via the
    // tab buttons (programmatic scroll), and each page still scrolls vertically.
    lv_obj_clear_flag(lv_tabview_get_content(tv), LV_OBJ_FLAG_SCROLLABLE);

    for (JsonVariantConst tabv : node["children"].as<JsonArrayConst>()) {
      JsonObjectConst tab = tabv.as<JsonObjectConst>();
      const char* title = tab["title"] | "Tab";
      lv_obj_t* page = lv_tabview_add_tab(tv, title);

      // Stack the tab content vertically with a little spacing.
      lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN);
      lv_obj_set_style_pad_row(page, 8, 0);
      // Scroll tracks the finger 1:1 (no overshoot/momentum animation).
      lv_obj_clear_flag(page,
                        LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);

      for (JsonVariantConst childv : tab["children"].as<JsonArrayConst>())
        ctx.buildNode(page, childv.as<JsonObjectConst>());
    }
    return tv;
  }

  bool buildsOwnChildren() const override { return true; }

 private:
  static void snapNoAnim(lv_event_t* e) {
    lv_obj_t* tv = static_cast<lv_obj_t*>(lv_event_get_user_data(e));
    lv_obj_t* btns = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(btns);
    if (id != LV_BTNMATRIX_BTN_NONE) lv_tabview_set_act(tv, id, LV_ANIM_OFF);
  }
};

}  // namespace ui
