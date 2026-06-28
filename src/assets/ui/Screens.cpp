/**
 * Screens.cpp - The demo UI as a JSON schema. Edit the UI here (or in /ui/*.json
 * on the SD card) WITHOUT touching C++.
 *
 * "{...}" tokens are replaced by Bindings at build time (e.g. "{sys.heap}").
 * "onClick"/"onChange" are action names wired to C++ callbacks via the ActionBus.
 * "id" lets a C++ action find a widget later (e.g. to update a status label).
 *
 * Root layout: a full-screen container holding the tabview plus a hidden on-screen
 * keyboard that overlays the bottom when a text field is tapped.
 */
#include "Screens.h"

namespace assets {

const char kDemoScreen[] = R"json(
{
  "type": "screen", "style": "bg",
  "children": [

    { "type": "tabview", "tab_height": 44, "width": "100%", "height": "100%",
      "children": [

      { "title": "Dashboard", "children": [
        { "type": "label", "text": "ESP32 LVGL Platform", "style": "title" },
        { "type": "label", "text": "{board.name}", "style": "subtitle" },
        { "type": "panel", "style": "card", "width": "100%", "height": "content",
          "flex": "column", "gap": 4, "children": [
            { "type": "label", "text": "Chip   : {sys.chip}  ({sys.cores} cores @ {sys.cpu} MHz)" },
            { "type": "label", "text": "Flash  : {sys.flash} MB" },
            { "type": "label", "text": "PSRAM  : {sys.psram} KB free" },
            { "type": "label", "text": "Heap   : {sys.heap} KB free", "style": "value" },
            { "type": "label", "text": "SDK    : {sys.sdk}" },
            { "type": "label", "text": "MAC    : {sys.mac}" }
          ]
        },
        { "type": "panel", "width": "100%", "height": "content", "flex": "row", "gap": 10, "children": [
          { "type": "led", "color": "#33FF66", "on": true },
          { "type": "label", "text": "System running", "grow": 1 },
          { "type": "button", "text": "Refresh", "onClick": "ui.refresh" },
          { "type": "button", "text": "Dashboard", "onClick": "nav.go:dashboard" }
        ]}
      ]},

      { "title": "Network", "children": [
        { "type": "label", "text": "Wi-Fi", "style": "title" },
        { "type": "panel", "style": "card", "width": "100%", "height": "content",
          "flex": "column", "gap": 4, "children": [
            { "type": "label", "text": "Status : {wifi.status}", "id": "wifi_status" },
            { "type": "label", "text": "IP     : {wifi.ip}", "id": "wifi_ip" }
          ]
        },
        { "type": "label", "text": "Tap Scan, then pick a network. The password box appears only for secured networks.",
          "style": "subtitle", "wrap": true, "width": "100%" },
        { "type": "button", "text": "Scan networks", "onClick": "wifi.scan", "width": "100%" },
        { "type": "label", "text": "Selected: (none)", "id": "wifi_selected", "wrap": true, "width": "100%" },
        { "type": "textarea", "id": "wifi_pass", "placeholder": "Password (tap to type)",
          "one_line": true, "password": true, "width": "100%", "onClick": "kb.show", "hidden": true },
        { "type": "button", "text": "Connect", "id": "wifi_connect", "onClick": "wifi.connect",
          "width": "100%", "hidden": true },
        { "type": "list", "id": "wifi_list", "width": "100%", "height": 150 }
      ]},

      { "title": "Storage", "children": [
        { "type": "label", "text": "SD Card", "style": "title" },
        { "type": "panel", "style": "card", "width": "100%", "height": "content", "children": [
          { "type": "label", "text": "Status : {sd.status}", "id": "sd_status" }
        ]},
        { "type": "button", "text": "Run read/write test", "onClick": "sd.test", "width": "100%" },
        { "type": "label", "text": "No test run yet", "id": "sd_result", "wrap": true, "width": "100%" }
      ]},

      { "title": "Controls", "children": [
        { "type": "label", "text": "Brightness" },
        { "type": "slider", "min": 20, "max": 255, "value": 255, "width": "100%", "onChange": "ui.brightness" },
        { "type": "label", "text": "Volume" },
        { "type": "slider", "min": 0, "max": 100, "value": 60, "width": "100%", "onChange": "audio.volume" },
        { "type": "button", "text": "Play tone", "onClick": "audio.play", "width": "100%" },
        { "type": "panel", "width": "100%", "height": "content", "flex": "row", "gap": 12, "children": [
          { "type": "label", "text": "Mode", "grow": 1 },
          { "type": "dropdown", "options": ["Auto", "Manual", "Off"], "selected": 0, "onChange": "ctrl.mode" }
        ]},
        { "type": "checkbox", "text": "Enable experimental feature", "checked": false, "onChange": "ctrl.experimental" },
        { "type": "panel", "width": "100%", "height": "content", "flex": "row", "gap": 12, "children": [
          { "type": "label", "text": "Background task", "grow": 1 },
          { "type": "switch", "checked": true, "onChange": "ctrl.bgtask" }
        ]},
        { "type": "arc", "min": 0, "max": 100, "value": 40, "width": 110, "height": 110, "align": "center" }
      ]},

      { "title": "System", "children": [
        { "type": "panel", "style": "card", "width": "100%", "height": "content", "flex": "column", "gap": 6, "children": [
          { "type": "panel", "width": "100%", "height": "content", "flex": "row", "gap": 12, "children": [
            { "type": "label", "text": "BLE advertising", "grow": 1 },
            { "type": "switch", "onChange": "ble.toggle" }
          ]},
          { "type": "label", "text": "BLE: idle", "id": "ble_status", "style": "subtitle" }
        ]},
        { "type": "panel", "style": "card", "width": "100%", "height": "content", "flex": "column", "gap": 6, "children": [
          { "type": "button", "text": "Check OTA update", "onClick": "ota.check", "width": "100%" },
          { "type": "label", "text": "OTA: -", "id": "ota_status", "style": "subtitle" }
        ]},
        { "type": "panel", "style": "card", "width": "100%", "height": "content", "flex": "column", "gap": 6, "children": [
          { "type": "button", "text": "Save settings to NVS", "onClick": "nvs.save", "width": "100%" },
          { "type": "label", "text": "-", "id": "save_status", "style": "subtitle" }
        ]},
        { "type": "button", "text": "Reboot device", "onClick": "system.reboot", "width": "100%", "bg": "#FF4455" }
      ]},

      { "title": "About", "children": [
        { "type": "label", "text": "About", "style": "title" },
        { "type": "label", "wrap": true, "width": "100%",
          "text": "A SOLID, multi-board LVGL platform for ESP32. Hardware is abstracted behind a HAL, features toggle via macros, and this whole UI is described by JSON - not hard-coded." },
        { "type": "label", "text": "Platform features:", "style": "subtitle" },
        { "type": "list", "width": "100%", "height": 160, "items": [
          { "text": "Display + Touch HAL (RGB / SPI)" },
          { "text": "Wi-Fi service" },
          { "text": "SD storage" },
          { "text": "NVS key-value store" },
          { "text": "OTA update (optional)" },
          { "text": "BLE (optional)" },
          { "text": "Audio speaker / mic (optional)" },
          { "text": "JSON-driven UI engine" }
        ]}
      ]}

    ]},

    { "type": "keyboard", "id": "kb", "width": "100%", "height": "40%",
      "align": "bottom_mid", "hidden": true, "onReady": "kb.hide", "onCancel": "kb.hide" }

  ]
}
)json";

// ---------------------------------------------------------------------------
// GPS performance meter dashboard (800x480). The card chrome, dividers, glow and
// the green/blue icons are baked into the "background" image; here we only overlay
// the dynamic text at absolute (x, y) positions measured against that image.
// "{race.*}" bindings supply the values; "{sym.*}" expand to LVGL symbol glyphs.
// ---------------------------------------------------------------------------
const char kRaceDashboard[] = R"json(
{
  "type": "screen", "bg_img": "background", "pad": 0, "radius": 0,
  "children": [

    { "type": "label", "text": "{sym.gps}   {sym.wifi}", "style": "val_green", "x": 20, "y": 46 },
    { "type": "label", "text": "12 SATELLITES", "style": "sect", "text_color": "#35DD5B", "x": 20, "y": 74 },

    { "type": "label", "text": "READY TO START", "style": "ready", "x": 20, "y": 112 },
    { "type": "label", "text": "Waiting for launch...", "style": "subtitle", "x": 20, "y": 142 },

    { "type": "button", "id": "start_btn", "style": "start_text", "text": "START",
      "x": 18, "y": 188, "width": 224, "height": 102, "onClick": "race.start" },

    { "type": "label", "id": "speed", "text": "{race.speed}", "style": "num_xl", "live": true, "x": 50, "y": 326 },
    { "type": "label", "text": "mph", "style": "unit", "x": 96, "y": 362 },

    { "type": "button", "style": "start_text", "text": "{sym.settings}",
      "x": 20, "y": 446, "width": 44, "height": 30, "onClick": "nav.go:demo" },

    { "type": "label", "text": "RUN #12", "style": "badge", "align": "top_right", "x": -30, "y": 12 },

    { "type": "label", "text": "0 - 60 mph", "style": "sect", "x": 292, "y": 58 },
    { "type": "label", "text": "3.42 s", "style": "val_green", "align": "top_right", "x": -32, "y": 56 },

    { "type": "label", "text": "60 ft", "style": "sect", "x": 292, "y": 106 },
    { "type": "label", "text": "1.62 s", "style": "val", "align": "top_right", "x": -32, "y": 104 },

    { "type": "label", "text": "330 ft", "style": "sect", "x": 292, "y": 155 },
    { "type": "label", "text": "4.48 s", "style": "val", "align": "top_right", "x": -32, "y": 153 },

    { "type": "label", "text": "1/8 mile", "style": "sect", "x": 292, "y": 204 },
    { "type": "label", "text": "6.88 s   @ 105.2 mph", "style": "val", "align": "top_right", "x": -32, "y": 202 },

    { "type": "label", "text": "1/4 mile", "style": "sect", "x": 292, "y": 253 },
    { "type": "label", "text": "10.65 s   @ 129.8 mph", "style": "val", "align": "top_right", "x": -32, "y": 251 },

    { "type": "label", "text": "129.8", "style": "num_lg", "x": 292, "y": 338 },
    { "type": "label", "text": "mph", "style": "unit", "x": 294, "y": 388 },

    { "type": "label", "text": "496", "style": "num_lg", "x": 556, "y": 338 },
    { "type": "label", "text": "hp", "style": "unit", "x": 558, "y": 388 },

    { "type": "label", "text": "2:35 PM", "style": "subtitle", "x": 292, "y": 452 },
    { "type": "label", "text": "86 F", "style": "subtitle", "x": 600, "y": 452 },
    { "type": "label", "text": "{sym.charge} 13.2V", "style": "subtitle", "align": "top_right", "x": -30, "y": 452 }
  ]
}
)json";

}  // namespace assets
