# utils

Developer helper scripts (not compiled into the firmware).

## img2c.py — image → LVGL `.c`

Converts an image into an LVGL 8.x image descriptor (`lv_img_dsc_t`) embedded in
flash as RGB565 true color (matches `LV_COLOR_DEPTH=16`, `LV_COLOR_16_SWAP=0` in
[include/lv_conf.h](../include/lv_conf.h)). The JSON UI references it by name through
the image registry: `"bg_img": "<name>"` / `"src": "<name>"`.

Requires Pillow: `pip install pillow` (NumPy optional, speeds it up).

```bash
# Whole image resized to the panel resolution
python utils/img2c.py src/assets/images/foo.png -W 800 -H 480

# Regenerate the dashboard background (crop inner panel, resize, name the symbol)
python utils/img2c.py src/assets/images/background.png \
    --crop 109,84,1162,724 -W 800 -H 480 \
    -o src/assets/images/background_img.c --name background_img
```

Then declare the symbol in a header (see
[src/assets/images/Images.h](../src/assets/images/Images.h)) and register it once at
startup in `main.cpp`:

```cpp
g_ui.registerImage("background", &background_img);
```

Flags: `--crop L,T,R,B`, `-W/--width`, `-H/--height` (one keeps aspect),
`--name`, `-o/--output`, `--swap` (only if `LV_COLOR_16_SWAP=1`).
