# Multi-board LVGL platform for ESP32 (SOLID)
# Nền tảng LVGL đa board cho ESP32 (SOLID)

**EN** — A reusable LVGL platform that runs across many ESP32 variants (ESP32 /
ESP32-C3 / ESP32-S3), many display & touch drivers, and different RAM/Flash sizes.
Features toggle with `#define` macros. The UI is **declared in JSON** instead of
hard-coded. Core idea: **adding new hardware = writing a class, never editing
existing code** (Open/Closed).

**VI** — Một nền tảng LVGL tái sử dụng, chạy trên nhiều dòng ESP32 (ESP32 /
ESP32-C3 / ESP32-S3), nhiều driver màn hình & cảm ứng, nhiều mức RAM/Flash. Bật/tắt
tính năng bằng macro `#define`. Giao diện **khai báo bằng JSON** thay vì hard-code.
Tư tưởng cốt lõi: **thêm phần cứng mới = viết thêm class, không sửa code cũ**
(Open/Closed).

---

## Architecture / Kiến trúc

```
                 ┌─────────────────────────────────────────────┐
   UI (LVGL)     │  ui/  UiFactory · ComponentRegistry          │  JSON schema -> widgets
                 │       StyleRegistry · ActionBus · Bindings    │  (assets/ui/*.json)
                 ├─────────────────────────────────────────────┤
   Platform      │  platform/  Platform · PlatformBuilder        │  assembles services per FEATURE_*
                 │  services/  WiFi · SD · NVS · OTA · BLE · …    │  each service behind an interface (DIP)
                 ├─────────────────────────────────────────────┤
   LVGL bridge   │  lvgl_port/  LvglPort  (the ONLY place that   │  takes IBoard, wires display+touch
                 │              knows both LVGL and the HAL)      │
                 ├─────────────────────────────────────────────┤
   HAL           │  hal/  IBoard · IDisplay · ITouch (interfaces)│  raw pixels, no LVGL
                 │  drivers/  LovyanGfxDisplay · Gt911 · …        │  one class per driver
                 │  boards/   CrowPanelS3Board · …  + Registry    │  one class per board
                 └─────────────────────────────────────────────┘
```

**EN** — Each layer depends only on the **interfaces** of the layer below, never on
a concrete class. `main.cpp` is the **composition root** — the only place that wires
concrete parts together.

**VI** — Mỗi tầng chỉ phụ thuộc **interface** của tầng dưới, không phụ thuộc lớp cụ
thể. `main.cpp` là **composition root** — nơi duy nhất ráp các lớp cụ thể lại.

## Directory layout / Cây thư mục

```
src/
  main.cpp                  Composition root: pick board, assemble platform, build first screen
  platform/
    Features.h              Feature-macro table (defaults; override in platformio.ini)
    Platform.{h,cpp}        Holder of board + services
    PlatformBuilder.{h,cpp} Assembles services per #if FEATURE_*  (DIP)
  hal/                      IBoard / IDisplay / ITouch / TouchPoint  (interfaces only)
  drivers/
    display/LovyanGfxDisplay.h   IDisplay adapter shared by RGB & SPI panels
    touch/Gt911Touch.*           GT911 (I2C)   — gated by DRIVER_GT911
    touch/LovyanTouch.h          touch integrated in a LovyanGFX device (XPT2046, FT5x06…)
  boards/
    BoardRegistry.h         picks ActiveBoard via a BOARD_* macro
    crowpanel_s3/           CrowPanel ESP32-S3 5.0" (RGB 800x480 + GT911)
    esp32c3_ili9341/        Sample board: ESP32-C3 + ILI9341 SPI + XPT2046 (not HW-tested)
  lvgl_port/LvglPort.*      HAL <-> LVGL bridge (board-agnostic)
  services/
    net/    IWifiService + ArduinoWifiService
    storage/ISdStorage + SdCard
    nvs/    IKeyValueStore + NvsKeyValueStore
    ota/    IOtaService + ArduinoOtaService     (FEATURE_OTA)
    ble/    IBleService + NimBleService          (FEATURE_BLE)
    audio/  ISpeaker/IMicrophone + I2s*          (FEATURE_AUDIO_*)
    system/ ISystemInfo + Esp32SystemInfo
  ui/
    UiFactory.*             JSON -> LVGL widget tree (recursive + applyCommon)
    UiContext.*             bundles Component/Style/Action/Binding registries
    Theme.h                 palette/fonts + named styles
    components/             IComponentBuilder, ComponentRegistry, builders/*
  assets/ui/Screens.*       JSON schema of the demo UI (embedded in flash)
include/lv_conf.h           LVGL 8.3.6 configuration
```

## Build

> **EN** — On Windows, run PlatformIO from **PowerShell/cmd**, NOT Git Bash/MSys —
> the toolchain installer (`idf_tools.py`) refuses to run under MSys/Mingw, so it
> cannot install a new toolchain (e.g. RISC-V for the ESP32-C3).
>
> **VI** — Trên Windows hãy chạy PlatformIO qua **PowerShell/cmd**, KHÔNG qua Git
> Bash/MSys — trình cài toolchain (`idf_tools.py`) từ chối chạy dưới MSys/Mingw nên
> không cài được toolchain mới (vd RISC-V cho ESP32-C3).

```powershell
# Real board CrowPanel-S3 / Board thật CrowPanel-S3
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e crowpanel_s3 -t upload

# Sample board ESP32-C3 (compile-check; sample pins, adjust before flashing)
# Board mẫu ESP32-C3 (chỉ compile; pin mẫu, chỉnh trước khi nạp)
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e esp32c3_ili9341
```

## Feature macros / Bảng macro tính năng (`src/platform/Features.h`)

| Macro                  | Default | Meaning / Ý nghĩa                                |
|------------------------|:-------:|--------------------------------------------------|
| `FEATURE_SD`           | 1       | SD card (SPI) / Thẻ SD (SPI)                      |
| `FEATURE_NVS`          | 1       | Persistent key-value (NVS) / Key-value bền vững   |
| `FEATURE_WIFI`         | 1       | Wi-Fi STA                                         |
| `FEATURE_OTA`          | 0       | OTA update (needs 2 app slots) / OTA (cần 2 slot) |
| `FEATURE_BLE`          | 0       | BLE (NimBLE — added only when on)                 |
| `FEATURE_AUDIO_SPEAKER`| 0       | I2S speaker (skeleton) / Loa I2S                  |
| `FEATURE_AUDIO_MIC`    | 0       | I2S mic (skeleton) / Mic I2S                      |
| `FEATURE_SYSTEM_INFO`  | 1       | Chip/heap info for UI bindings                    |
| `FEATURE_UI_FROM_SD`   | 0       | Load JSON UI from SD instead of PROGMEM           |
| `DRIVER_GT911`         | 0       | Compile the GT911 driver (env with its lib)       |

Override per board in `platformio.ini`, e.g. `-D FEATURE_BLE=1`.
Ghi đè theo board trong `platformio.ini`, ví dụ `-D FEATURE_BLE=1`.

---

# How to add a new device / Cách thêm một device mới

**EN** — A "device" = an ESP32 board with its display + touch. Adding one touches
**no existing file**; you only create new files and add one registry branch + one
build env.

**VI** — Một "device" = một board ESP32 với màn hình + cảm ứng của nó. Thêm device
**không đụng vào file cũ nào**; bạn chỉ tạo file mới và thêm 1 nhánh registry + 1 env
build.

### Step 1 — Describe the display hardware / Mô tả phần cứng màn hình

**EN** — Create a LovyanGFX device class for your panel under
`src/boards/<your_board>/LGFX_<Panel>.h` (set the bus, panel, backlight, and touch
if it is on the same SPI bus). See `boards/crowpanel_s3/LGFX_CrowPanel.h` (RGB) or
`boards/esp32c3_ili9341/LGFX_Ili9341.h` (SPI + XPT2046) as templates.

**VI** — Tạo một class thiết bị LovyanGFX cho panel của bạn tại
`src/boards/<board_cua_ban>/LGFX_<Panel>.h` (cấu hình bus, panel, đèn nền, và touch
nếu chung bus SPI). Tham khảo `LGFX_CrowPanel.h` (RGB) hoặc `LGFX_Ili9341.h`
(SPI + XPT2046) làm mẫu.

### Step 2 — Provide a touch driver if needed / Cung cấp driver cảm ứng nếu cần

**EN** — Reuse an existing `hal::ITouch`:
- `drivers/touch/LovyanTouch.h` — for touch integrated in the LovyanGFX device
  (XPT2046, FT5x06…); **no extra library**.
- `drivers/touch/Gt911Touch.*` — GT911 over I2C (needs the `TAMC_GT911` lib +
  `-D DRIVER_GT911=1`).

If your controller is new, add `drivers/touch/<Yours>Touch.{h,cpp}` implementing
`hal::ITouch`. If the controller needs an external library, wrap the whole `.h`/`.cpp`
in `#if DRIVER_<YOURS>` and add the library only to that env (otherwise the file
fails to compile on boards that lack the library).

**VI** — Tái dùng một `hal::ITouch` có sẵn:
- `LovyanTouch.h` — cho touch tích hợp trong thiết bị LovyanGFX (XPT2046, FT5x06…);
  **không cần thư viện ngoài**.
- `Gt911Touch.*` — GT911 qua I2C (cần lib `TAMC_GT911` + `-D DRIVER_GT911=1`).

Nếu controller mới, thêm `drivers/touch/<Cua_ban>Touch.{h,cpp}` implement
`hal::ITouch`. Nếu cần thư viện ngoài, bọc toàn bộ `.h`/`.cpp` trong
`#if DRIVER_<CUA_BAN>` và chỉ thêm lib vào env đó (nếu không, file sẽ lỗi biên dịch
trên board không có lib).

### Step 3 — Write the board class / Viết class board

`src/boards/<your_board>/<YourBoard>.h`:

```cpp
#pragma once
#include "../../drivers/display/LovyanGfxDisplay.h"
#include "../../drivers/touch/LovyanTouch.h"   // or your touch driver
#include "../../hal/IBoard.h"
#include "LGFX_MyPanel.h"

namespace boards {

class MyBoard : public hal::IBoard {
 public:
  const char* name() const override { return "My Board"; }
  bool begin() override { bool ok = display_.begin(); touch_.begin(); return ok; }
  hal::IDisplay& display() override { return display_; }
  hal::ITouch&   touch()   override { return touch_; }
 private:
  static constexpr int16_t kW = 320, kH = 240;
  LGFX_MyPanel lcd_;
  drivers::LovyanGfxDisplay display_{lcd_, kW, kH};
  drivers::LovyanTouch      touch_{lcd_};
};

}  // namespace boards
```

### Step 4 — Register the board / Đăng ký board

Add one branch in `src/boards/BoardRegistry.h`:
Thêm một nhánh vào `src/boards/BoardRegistry.h`:

```cpp
#elif defined(BOARD_MY_BOARD)
#include "my_board/MyBoard.h"
namespace boards { using ActiveBoard = MyBoard; }
```

### Step 5 — Add a build env / Thêm env build

In `platformio.ini`:

```ini
[env:my_board]
extends = common
board = esp32dev            ; or your board id
build_flags =
	${common.build_flags}
	-D BOARD_MY_BOARD
	; -D BOARD_HAS_PSRAM     ; if the chip has PSRAM
	; -D DRIVER_GT911=1      ; if using GT911
	; -D FEATURE_BLE=1 ...   ; enable/disable features
; lib_deps = ${common.lib_deps}   ; + extra libs if needed
; board_build.partitions = partitions_default.csv  ; if the default app slot is too small
```

**Done.** `LvglPort`, `services/`, `ui/` are untouched. Build with
`pio run -e my_board`.
**Xong.** `LvglPort`, `services/`, `ui/` không hề bị sửa. Build với
`pio run -e my_board`.

---

## How to add a UI component / Cách thêm UI component

1. Create `src/ui/components/builders/<Name>Builder.h` implementing
   `ui::IComponentBuilder` (build an LVGL widget from a `JsonObjectConst node`).
2. Register it in `src/ui/components/BuiltinComponents.h`:
   `reg.add("type_name_in_schema", &builder);`

`UiFactory` is untouched — it looks builders up via the `ComponentRegistry`.
`UiFactory` không bị sửa — nó tra builder qua `ComponentRegistry`.

## JSON UI schema / Cú pháp UI JSON

A node = `{ "type": "...", <props>, "children": [ ... ] }`.

**Common props** (applied by UiFactory): `style` (name or array), `bg`,
`text_color` (`"#RRGGBB"`), `radius`, `pad`, `border`, `width`/`height`
(px · `"100%"` · `"content"`), `flex` (`"row"`/`"column"`), `gap`, `grow`,
`align` (`center`, `top_left`…), `x`/`y`, `id`.

**Widgets**: `screen`/`container`/`panel`, `label`, `button`, `slider`, `switch`,
`checkbox`, `dropdown`, `roller`, `arc`, `bar`, `led`, `spinner`, `textarea`,
`list`, `image`, `tabview` (children are `{ "title", "children" }` tabs).

- **Binding**: `{key}` tokens in `text` are replaced by a registered provider
  (e.g. `{sys.heap}`). Register via `ctx.bindings().provide("key", lambda)`.
- **Action**: `onClick`/`onChange` are **action names** wired to C++ via
  `ctx.actions().on("name", lambda)` → the schema never names a C++ function.
- **id**: lets a C++ action find a widget later with `ctx.byId("id")`.

Edit the UI by editing `src/assets/ui/Screens.cpp` (or `/ui/*.json` on SD when
`FEATURE_UI_FROM_SD=1`), **not** C++.
Sửa giao diện bằng cách sửa `src/assets/ui/Screens.cpp` (hoặc `/ui/*.json` trên SD
khi `FEATURE_UI_FROM_SD=1`), **không** sửa C++.

## Demo UI

`assets/ui/Screens.cpp` defines a tabview demo with tabs **Dashboard / Network /
Storage / Controls / System / About** that exercises every service (Wi-Fi scan, SD
read/write test, NVS save, OTA check, BLE advertise toggle, brightness/volume,
reboot) and a wide set of widgets — all from JSON.

`assets/ui/Screens.cpp` định nghĩa demo tabview với các tab **Dashboard / Network /
Storage / Controls / System / About**, dùng thử mọi service (quét Wi-Fi, test
đọc/ghi SD, lưu NVS, kiểm tra OTA, bật/tắt quảng bá BLE, độ sáng/âm lượng, reboot)
và nhiều loại widget — tất cả từ JSON.
