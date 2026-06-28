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
          { "type": "button", "text": "Refresh", "onClick": "ui.refresh" }
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
        { "type": "label", "text": "Scan, pick a network below, type the password, then Connect.",
          "style": "subtitle", "wrap": true, "width": "100%" },
        { "type": "textarea", "id": "wifi_pass", "placeholder": "Password (tap to type)",
          "one_line": true, "password": true, "width": "100%", "onClick": "kb.show" },
        { "type": "panel", "width": "100%", "height": "content", "flex": "row", "gap": 10, "children": [
          { "type": "button", "text": "Scan", "onClick": "wifi.scan", "grow": 1 },
          { "type": "button", "text": "Connect", "onClick": "wifi.connect", "grow": 1 }
        ]},
        { "type": "label", "text": "Selected: (none)", "id": "wifi_selected", "wrap": true, "width": "100%" },
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

    { "type": "keyboard", "id": "kb", "width": "100%", "height": "45%",
      "align": "bottom_mid", "hidden": true, "onReady": "kb.hide", "onCancel": "kb.hide" }

  ]
}
)json";

}  // namespace assets
