/**
 * DemoPage.cpp - The original platform feature-demo (Dashboard / Network / Storage /
 * Controls / System / About tabview). All of its bindings and actions - which used to
 * crowd main.cpp - now live here, reaching services through AppContext::platform.
 * Self-registers; reachable from the dashboard via "nav.go:demo".
 */
#include <Arduino.h>
#include <FS.h>
#include <lvgl.h>

#include <vector>

#include "../../assets/ui/Screens.h"
#include "../../lvgl_port/LvglPort.h"
#include "../../platform/Platform.h"
#include "../../services/audio/ISpeaker.h"
#include "../../services/ble/IBleService.h"
#include "../../services/net/IWifiService.h"
#include "../../services/nvs/IKeyValueStore.h"
#include "../../services/ota/IOtaService.h"
#include "../../services/storage/ISdStorage.h"
#include "../../services/system/ISystemInfo.h"
#include "../../ui/UiContext.h"
#include "../../ui/UiFactory.h"
#include "../AppRegister.h"
#include "../Router.h"
#include "IPage.h"

namespace app {

class DemoPage : public IPage {
 public:
  const char* id() const override { return "demo"; }

  void onRegister(AppContext& ctx) override {
    ctx_ = &ctx;
    registerBindings();
    registerActions();
  }

  void onShow(lv_obj_t* root, AppContext& ctx) override {
    ui::UiFactory factory(ctx.ui);
    factory.buildFromJson(root, assets::kDemoScreen);
  }

 private:
  ui::UiContext& ui() { return ctx_->ui; }
  platform::Platform& plat() { return ctx_->platform; }

  void setLabel(const char* id, const String& text) {
    if (lv_obj_t* o = ui().byId(id)) lv_label_set_text(o, text.c_str());
  }

  void registerBindings() {
    sys::ISystemInfo* info = plat().systemInfo();
    net::IWifiService* wifi = plat().wifi();
    storage::ISdStorage* sd = plat().sd();
    const char* boardName = plat().board().name();

    auto& b = ui().bindings();
    b.provide("board.name", [boardName] { return String(boardName); });
    b.provide("sys.chip",  [info] { return info ? info->chipModel() : String("?"); });
    b.provide("sys.cores", [info] { return info ? String(info->chipCores()) : String("?"); });
    b.provide("sys.cpu",   [info] { return info ? String(info->cpuFreqMhz()) : String("?"); });
    b.provide("sys.heap",  [info] { return info ? String(info->freeHeapBytes() / 1024u) : String("?"); });
    b.provide("sys.psram", [info] { return info ? String(info->freePsramBytes() / 1024u) : String("?"); });
    b.provide("sys.flash", [info] { return info ? String(info->flashSizeBytes() / (1024u * 1024u)) : String("?"); });
    b.provide("sys.sdk",   [info] { return info ? info->sdkVersion() : String("?"); });
    b.provide("sys.mac",   [info] { return info ? info->macAddress() : String("?"); });
    b.provide("wifi.status", [wifi] {
      if (!wifi) return String("disabled");
      return wifi->isConnected() ? String("connected") : String("not connected");
    });
    b.provide("wifi.ip", [wifi] {
      return (wifi && wifi->isConnected()) ? wifi->ipAddress() : String("-");
    });
    b.provide("sd.status", [sd] {
      if (!sd) return String("disabled");
      return sd->mounted() ? String("mounted") : String("not mounted");
    });
  }

  void registerActions() {
    auto& a = ui().actions();

    a.on("ui.refresh", [this](lv_event_t*) { ctx_->router.show("demo"); });

    a.on("ui.brightness", [this](lv_event_t* e) {
      brightness_ = lv_slider_get_value(lv_event_get_target(e));
      lvgl_port::setBacklight((uint8_t)brightness_);
    });

    a.on("audio.volume", [this](lv_event_t* e) {
      const int v = lv_slider_get_value(lv_event_get_target(e));
      if (auto* sp = plat().speaker()) sp->setVolume((uint8_t)v);
      Serial.printf("[Audio] volume = %d\n", v);
    });
    a.on("audio.play", [this](lv_event_t*) {
      if (plat().speaker())
        Serial.println("[Audio] play tone (speaker present)");
      else
        Serial.println("[Audio] speaker disabled (FEATURE_AUDIO_SPEAKER=0)");
    });

    a.on("ctrl.mode", [](lv_event_t* e) {
      char buf[24];
      lv_dropdown_get_selected_str(lv_event_get_target(e), buf, sizeof(buf));
      Serial.printf("[Ctrl] mode = %s\n", buf);
    });
    a.on("ctrl.experimental", [](lv_event_t* e) {
      Serial.printf("[Ctrl] experimental = %d\n",
                    lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
    });
    a.on("ctrl.bgtask", [](lv_event_t* e) {
      Serial.printf("[Ctrl] background task = %d\n",
                    lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
    });

    // Show/hide the on-screen keyboard and link it to the tapped textarea.
    a.on("kb.show", [this](lv_event_t* e) {
      lv_obj_t* ta = lv_event_get_target(e);
      if (lv_obj_t* kb = ui().byId("kb")) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb);
      }
    });
    a.on("kb.hide", [this](lv_event_t*) {
      if (lv_obj_t* kb = ui().byId("kb")) lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    });

    a.on("wifi.scan", [this](lv_event_t*) { wifiScan(); });
    a.on("wifi.connect", [this](lv_event_t*) { wifiConnect(); });

    a.on("sd.test", [this](lv_event_t*) { sdTest(); });

    a.on("nvs.save", [this](lv_event_t*) {
      auto* kv = plat().nvs();
      if (!kv) { setLabel("save_status", "NVS disabled (FEATURE_NVS=0)"); return; }
      kv->begin("ui");
      kv->putInt("brightness", brightness_);
      kv->putBool("saved", true);
      setLabel("save_status", "Saved (brightness=" + String(brightness_) + ")");
    });

    a.on("ota.check", [this](lv_event_t*) {
      auto* ota = plat().ota();
      if (!ota) { setLabel("ota_status", "OTA disabled (FEATURE_OTA=0)"); return; }
      setLabel("ota_status", String("OTA ready: ") + ota->lastMessage());
    });

    a.on("ble.toggle", [this](lv_event_t* e) {
      auto* ble = plat().ble();
      const bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
      if (!ble) { setLabel("ble_status", "BLE disabled (FEATURE_BLE=0)"); return; }
      if (on) {
        ble->begin("ESP32-Platform");
        setLabel("ble_status", "BLE: advertising");
      } else {
        ble->stop();
        setLabel("ble_status", "BLE: stopped");
      }
    });

    a.on("system.reboot", [](lv_event_t*) {
      Serial.println("[System] rebooting...");
      delay(300);
      ESP.restart();
    });
  }

  void wifiScan() {
    auto* wifi = plat().wifi();
    lv_obj_t* list = ui().byId("wifi_list");
    if (!wifi || !list) return;

    // Reset the selection flow: hide password + Connect until a network is picked.
    selectedSsid_ = "";
    if (lv_obj_t* s = ui().byId("wifi_selected"))
      lv_label_set_text(s, "Selected: (none)");
    if (lv_obj_t* ta = ui().byId("wifi_pass")) {
      lv_textarea_set_text(ta, "");
      lv_obj_add_flag(ta, LV_OBJ_FLAG_HIDDEN);
    }
    if (lv_obj_t* c = ui().byId("wifi_connect")) lv_obj_add_flag(c, LV_OBJ_FLAG_HIDDEN);
    if (lv_obj_t* kb = ui().byId("kb")) lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clean(list);
    lv_list_add_text(list, "Scanning...");
    lv_refr_now(NULL);  // paint "Scanning..." before the blocking scan

    scan_.clear();
    wifi->scan(scan_);

    lv_obj_clean(list);
    if (scan_.empty()) {
      lv_list_add_text(list, "No networks found");
      return;
    }
    for (size_t i = 0; i < scan_.size(); ++i) {
      const net::WifiNetwork& nw = scan_[i];
      String row = nw.ssid + "   " + nw.rssi + " dBm" + (nw.open ? "  (open)" : "");
      lv_obj_t* btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, row.c_str());
      // Remember the index on the button; the page itself is the event user_data.
      lv_obj_set_user_data(btn, (void*)(intptr_t)i);
      lv_obj_add_event_cb(btn, &DemoPage::onPickNetwork, LV_EVENT_CLICKED, this);
    }
  }

  // Static trampoline: a network row was tapped (idx stored in user_data, page in
  // the event user_data passed at add_event_cb).
  static void onPickNetwork(lv_event_t* e) {
    auto* self = static_cast<DemoPage*>(lv_event_get_user_data(e));
    size_t idx = (size_t)(intptr_t)lv_obj_get_user_data(lv_event_get_target(e));
    if (!self || idx >= self->scan_.size()) return;
    const net::WifiNetwork& nw = self->scan_[idx];
    self->selectedSsid_ = nw.ssid;
    if (lv_obj_t* l = self->ui().byId("wifi_selected"))
      lv_label_set_text(l, (String("Selected: ") + self->selectedSsid_ +
                            (nw.open ? "  (open, no password)" : "")).c_str());
    if (lv_obj_t* c = self->ui().byId("wifi_connect"))
      lv_obj_clear_flag(c, LV_OBJ_FLAG_HIDDEN);
    if (lv_obj_t* ta = self->ui().byId("wifi_pass")) {
      lv_textarea_set_text(ta, "");
      if (nw.open)
        lv_obj_add_flag(ta, LV_OBJ_FLAG_HIDDEN);    // open network: no password
      else
        lv_obj_clear_flag(ta, LV_OBJ_FLAG_HIDDEN);  // secured: ask for password
    }
  }

  void wifiConnect() {
    auto* wifi = plat().wifi();
    if (!wifi) { setLabel("wifi_selected", "Wi-Fi disabled"); return; }
    if (lv_obj_t* kb = ui().byId("kb")) lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (selectedSsid_.isEmpty()) {
      setLabel("wifi_selected", "Pick a network from the list first");
      return;
    }
    String pass;
    if (lv_obj_t* ta = ui().byId("wifi_pass")) pass = lv_textarea_get_text(ta);
    setLabel("wifi_selected", "Connecting to " + selectedSsid_ + "...");
    lv_refr_now(NULL);
    const bool ok = wifi->connect(selectedSsid_, pass, 12000);
    setLabel("wifi_selected", ok ? "Connected: " + wifi->ipAddress()
                                 : "Failed to connect");
    setLabel("wifi_status", String("Status : ") + (ok ? "connected" : "not connected"));
    setLabel("wifi_ip", String("IP     : ") + (ok ? wifi->ipAddress() : String("-")));
  }

  void sdTest() {
    auto* sd = plat().sd();
    if (!sd || !sd->mounted()) { setLabel("sd_result", "SD not mounted"); return; }
    fs::File w = sd->fs().open("/platform_test.txt", FILE_WRITE);
    if (!w) { setLabel("sd_result", "Open for write failed"); return; }
    w.println("hello from the LVGL platform");
    w.close();
    fs::File r = sd->fs().open("/platform_test.txt", FILE_READ);
    String content = r ? r.readStringUntil('\n') : String("(read failed)");
    if (r) r.close();
    setLabel("sd_result", "Wrote + read back: \"" + content + "\"");
  }

  AppContext* ctx_ = nullptr;
  int brightness_ = 255;
  std::vector<net::WifiNetwork> scan_;
  String selectedSsid_;
};

APP_REGISTER_PAGE(DemoPage);

}  // namespace app
