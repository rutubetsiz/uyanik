// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#include "press.h"
#include <esp_timer.h>

namespace press
{
static uint8_t pinPower = 255, pinReset = 255;
static volatile int8_t  active = -1;
static esp_timer_handle_t timer = nullptr;
static void (*releaseCb)() = nullptr;
static volatile bool releasedFlag = false;

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

static void release(void *)
{
  portENTER_CRITICAL(&mux);
  int8_t pin = active;
  portEXIT_CRITICAL(&mux);
  if (pin < 0) return;

  digitalWrite((uint8_t)pin, LOW);

  portENTER_CRITICAL(&mux);
  active = -1;
  portEXIT_CRITICAL(&mux);

  releasedFlag = true;
  if (releaseCb) releaseCb();
}

void begin(uint8_t powerPin, uint8_t resetPin)
{
  pinPower = powerPin;
  pinReset = resetPin;

  digitalWrite(pinPower, LOW);
  digitalWrite(pinReset, LOW);
  pinMode(pinPower, OUTPUT);
  pinMode(pinReset, OUTPUT);
  digitalWrite(pinPower, LOW);
  digitalWrite(pinReset, LOW);

  esp_timer_create_args_t args = {};
  args.callback = &release;
  args.dispatch_method = ESP_TIMER_TASK;
  args.name = "press";
  if (esp_timer_create(&args, &timer) != ESP_OK)
  {
    timer = nullptr;
    Serial.println("[press] ERROR: Timer could not be set; press is disabled");
  }
}

static bool start(uint8_t pin, uint32_t ms)
{
  if (timer == nullptr || pin == 255) return false;
  if (ms == 0 || ms > 15000) return false;

  portENTER_CRITICAL(&mux);
  bool taken = (active < 0);
  if (taken) active = (int8_t)pin;
  portEXIT_CRITICAL(&mux);
  if (!taken) return false;

  digitalWrite(pin, HIGH);
  esp_timer_stop(timer);
  if (esp_timer_start_once(timer, (uint64_t)ms * 1000) != ESP_OK)
  {
    digitalWrite(pin, LOW);
    portENTER_CRITICAL(&mux);
    active = -1;
    portEXIT_CRITICAL(&mux);
    return false;
  }
  Serial.printf("[press] pin %d / %u ms\n", pin, (unsigned)ms);
  return true;
}

bool power(uint32_t ms) {return start(pinPower, ms);}
bool reset(uint32_t ms) {return start(pinReset, ms);}

bool busy() {return active >= 0;}

bool takeRelease()
{
  if (!releasedFlag) return false;
  releasedFlag = false;
  return true;
}

void onRelease(void (*cb)()) {releaseCb = cb;}

}
