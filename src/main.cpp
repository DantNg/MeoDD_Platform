/**
 * main.cpp - Composition root of the multi-board LVGL platform.
 *
 * This is the ONE place that wires concrete parts together: pick the board
 * (BOARD_* via BoardRegistry), assemble the Platform (services per feature),
 * start the LVGL port, register the UI engine (styles/components/bindings/
 * actions), then build the first screen from a JSON schema.
 *
 * Everything above depends only on interfaces (IBoard, IWifiService,
 * IComponentBuilder, ...).
 */
#include <Arduino.h>
#include <FS.h>
#include <lvgl.h>

#include "assets/ui/Screens.h"
#include "boards/BoardRegistry.h"
#include "lvgl_port/LvglPort.h"
#include "platform/PlatformBuilder.h"
#include "services/ble/IBleService.h"
#include "services/audio/ISpeaker.h"
#include "services/net/IWifiService.h"
#include "services/nvs/IKeyValueStore.h"
#include "services/ota/IOtaService.h"
#include "services/storage/ISdStorage.h"
#include "services/system/ISystemInfo.h"
#include "ui/Theme.h"
#include "ui/UiContext.h"
#include "ui/UiFactory.h"
#include "ui/components/BuiltinComponents.h"

static boards::ActiveBoard s_board;
static platform::Platform* g_platform = nullptr;
static ui::UiContext g_ui;
static ui::UiFactory g_factory(g_ui);
static int g_brightness = 255;
static std::vector<net::WifiNetwork> g_scan;  // last Wi-Fi scan results
static String g_selectedSsid;                 // SSID picked from the scan list

// Build (or rebuild) the demo screen on the active LVGL screen.
static void showDemo() {
  lv_obj_t* scr = lv_scr_act();
  lv_obj_clean(scr);
  g_factory.buildFromJson(scr, assets::kDemoScreen);
}

// Update a label found by its schema "id".
static void setLabel(const char* id, const String& text) {
  if (lv_obj_t* o = g_ui.byId(id)) lv_label_set_text(o, text.c_str());
}

// --- Live data for "{...}" tokens in the schema ---
static void registerBindings() {
  sys::ISystemInfo* info = g_platform->systemInfo();
  net::IWifiService* wifi = g_platform->wifi();
  storage::ISdStorage* sd = g_platform->sd();

  auto& b = g_ui.bindings();
  b.provide("board.name", [] { return String(s_board.name()); });
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

// --- Map schema action names to C++ callbacks ---
static void registerActions() {
  auto& a = g_ui.actions();

  a.on("ui.refresh", [](lv_event_t*) { showDemo(); });

  a.on("ui.brightness", [](lv_event_t* e) {
    g_brightness = lv_slider_get_value(lv_event_get_target(e));
    lvgl_port::setBacklight((uint8_t)g_brightness);
  });

  a.on("audio.volume", [](lv_event_t* e) {
    const int v = lv_slider_get_value(lv_event_get_target(e));
    if (auto* sp = g_platform->speaker()) sp->setVolume((uint8_t)v);
    Serial.printf("[Audio] volume = %d\n", v);
  });
  a.on("audio.play", [](lv_event_t*) {
    if (auto* sp = g_platform->speaker())
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
  a.on("kb.show", [](lv_event_t* e) {
    lv_obj_t* ta = lv_event_get_target(e);
    if (lv_obj_t* kb = g_ui.byId("kb")) {
      lv_keyboard_set_textarea(kb, ta);
      lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(kb);
    }
  });
  a.on("kb.hide", [](lv_event_t*) {
    if (lv_obj_t* kb = g_ui.byId("kb")) lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  });

  a.on("wifi.scan", [](lv_event_t*) {
    auto* wifi = g_platform->wifi();
    lv_obj_t* list = g_ui.byId("wifi_list");
    if (!wifi || !list) return;
    lv_obj_clean(list);
    lv_list_add_text(list, "Scanning...");
    lv_refr_now(NULL);  // paint "Scanning..." before the blocking scan

    g_scan.clear();
    wifi->scan(g_scan);

    lv_obj_clean(list);
    if (g_scan.empty()) {
      lv_list_add_text(list, "No networks found");
      return;
    }
    for (size_t i = 0; i < g_scan.size(); ++i) {
      const net::WifiNetwork& nw = g_scan[i];
      String row = nw.ssid + "   " + nw.rssi + " dBm" + (nw.open ? "  (open)" : "");
      lv_obj_t* btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, row.c_str());
      lv_obj_set_user_data(btn, (void*)(intptr_t)i);  // remember which network
      // Pick this network as the connection target.
      lv_obj_add_event_cb(btn, [](lv_event_t* e) {
        size_t idx = (size_t)(intptr_t)lv_obj_get_user_data(lv_event_get_target(e));
        if (idx < g_scan.size()) {
          g_selectedSsid = g_scan[idx].ssid;
          if (lv_obj_t* l = g_ui.byId("wifi_selected"))
            lv_label_set_text(l, (String("Selected: ") + g_selectedSsid).c_str());
        }
      }, LV_EVENT_CLICKED, nullptr);
    }
  });

  a.on("wifi.connect", [](lv_event_t*) {
    auto* wifi = g_platform->wifi();
    if (!wifi) { setLabel("wifi_selected", "Wi-Fi disabled"); return; }
    if (lv_obj_t* kb = g_ui.byId("kb")) lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (g_selectedSsid.isEmpty()) {
      setLabel("wifi_selected", "Pick a network from the list first");
      return;
    }
    String pass;
    if (lv_obj_t* ta = g_ui.byId("wifi_pass")) pass = lv_textarea_get_text(ta);
    setLabel("wifi_selected", "Connecting to " + g_selectedSsid + "...");
    lv_refr_now(NULL);
    const bool ok = wifi->connect(g_selectedSsid, pass, 12000);
    setLabel("wifi_selected", ok ? "Connected: " + wifi->ipAddress()
                                 : "Failed to connect");
    setLabel("wifi_status", String("Status : ") + (ok ? "connected" : "not connected"));
    setLabel("wifi_ip", String("IP     : ") + (ok ? wifi->ipAddress() : String("-")));
  });

  a.on("sd.test", [](lv_event_t*) {
    auto* sd = g_platform->sd();
    if (!sd || !sd->mounted()) { setLabel("sd_result", "SD not mounted"); return; }
    fs::File w = sd->fs().open("/platform_test.txt", FILE_WRITE);
    if (!w) { setLabel("sd_result", "Open for write failed"); return; }
    w.println("hello from the LVGL platform");
    w.close();
    fs::File r = sd->fs().open("/platform_test.txt", FILE_READ);
    String content = r ? r.readStringUntil('\n') : String("(read failed)");
    if (r) r.close();
    setLabel("sd_result", "Wrote + read back: \"" + content + "\"");
  });

  a.on("nvs.save", [](lv_event_t*) {
    auto* kv = g_platform->nvs();
    if (!kv) { setLabel("save_status", "NVS disabled (FEATURE_NVS=0)"); return; }
    kv->begin("ui");
    kv->putInt("brightness", g_brightness);
    kv->putBool("saved", true);
    setLabel("save_status", "Saved (brightness=" + String(g_brightness) + ")");
  });

  a.on("ota.check", [](lv_event_t*) {
    auto* ota = g_platform->ota();
    if (!ota) { setLabel("ota_status", "OTA disabled (FEATURE_OTA=0)"); return; }
    setLabel("ota_status", String("OTA ready: ") + ota->lastMessage());
  });

  a.on("ble.toggle", [](lv_event_t* e) {
    auto* ble = g_platform->ble();
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

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf("\n[Platform] Board: %s\n", s_board.name());

  // 1) Hardware + LVGL (board-agnostic).
  if (!lvgl_port::begin(s_board)) {
    Serial.println("[Platform] ERROR: failed to allocate draw buffer!");
    while (true) delay(1000);
  }
  lvgl_port::setBacklight(255);

  // 2) Services per feature macro.
  g_platform = &platform::PlatformBuilder::build(s_board);
  g_platform->beginServices();

  // 3) UI engine: styles + components + bindings + actions.
  ui::theme::registerStyles(g_ui.styles());
  ui::registerBuiltinComponents(g_ui.components());
  registerBindings();
  registerActions();

  // 4) Build the first screen from the JSON schema (no hard-coded widgets).
  showDemo();

  Serial.println("[Platform] Ready.");
}

void loop() {
  lvgl_port::pump();
  delay(2);
}
