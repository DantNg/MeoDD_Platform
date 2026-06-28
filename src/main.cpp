/**
 * main.cpp - Entry point only. All wiring lives in app::App; all UI lives in pages
 * that self-register (src/app/pages), all background logic in self-registering
 * features (src/app/features). Adding a screen or a feature needs NO change here.
 */
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "app/App.h"
#include "boards/BoardRegistry.h"

static boards::ActiveBoard s_board;

void setup() { app::App::run(s_board); }

// Everything runs on the GUI task + feature tasks; the Arduino loop task idles.
void loop() { vTaskDelay(portMAX_DELAY); }
