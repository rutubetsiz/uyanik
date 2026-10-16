// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#include "identity.h"
// #include <WiFi.h>
#include <Preferences.h>
#include <esp_random.h>
#include <esp_timer.h>
#include <esp_mac.h>

/*
  Cihaz kimliği ve şifre üretimi
  Device identification and password generation
*/

namespace identity
{

static const char CHARSET[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz"; // Unambiguous character set, Base56
static const uint8_t CREDENTIAL_LENGTH = 12; 

static const uint32_t BUTTON_CHECK = 23;
static const uint16_t HOLD_SAMPLES = 217;
static const uint16_t NOTICE = 9;

static String ssid, defaultPassword, otaPassword;
static uint8_t buttonPin = 255;
static esp_timer_handle_t buttonTimer = nullptr;
static volatile uint16_t lowCount = 0;
static volatile bool resetRequest = false, holdNotice = false;

static String makeCredential()
{
  String s;
  s.reserve(CREDENTIAL_LENGTH);
  for (uint8_t i = 0; i < CREDENTIAL_LENGTH; i++)
  {
    s += CHARSET[esp_random() % (sizeof(CHARSET) - 1)];
  }
  return s;
}

static void sampleButton(void *)
{
  if (resetRequest) return;

  if (digitalRead(buttonPin) != LOW)
  {
    lowCount = 0;
    return;
  }

  if (lowCount < HOLD_SAMPLES) lowCount++;
  if (lowCount == NOTICE) holdNotice = true;
  if (lowCount >= HOLD_SAMPLES) resetRequest = true;
}

void begin(uint8_t resetButtonPin)
{
  buttonPin = resetButtonPin;
  pinMode(buttonPin, INPUT_PULLUP);

  esp_timer_create_args_t args = {};
  args.callback = &sampleButton;
  args.dispatch_method = ESP_TIMER_TASK;
  args.name = "button";

  if (esp_timer_create(&args, &buttonTimer) != ESP_OK || esp_timer_start_periodic(buttonTimer, (uint64_t)BUTTON_CHECK * 1000) != ESP_OK)
  {
    buttonTimer = nullptr;
    Serial.println("ERROR: Unable to set up the button timer; factory reset is disabled.");
  }

  /*
    WiFi ayağa kalkmadan MAC adresini okumamız lazım.
    "UYANIK-XXXX" formatında SSID üretmek için MAC adresi çekilir.

    We need to read the MAC address before it connects to WiFi.
    The MAC address is pulled to generate an SSID in the “UYANIK-XXXX” format.
  */

  uint8_t mac[6] = {0};
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  char buf[24];
  snprintf(buf, sizeof(buf), "UYANIK-%02X%02X", mac[4], mac[5]);
  ssid = buf;

  Preferences p;
  p.begin("uyanik-id", false);
  defaultPassword = p.getString("panel", "");
  otaPassword = p.getString("ota", "");

  if (defaultPassword.length() < CREDENTIAL_LENGTH || otaPassword.length() < CREDENTIAL_LENGTH)
  {
    defaultPassword = makeCredential();
    otaPassword = makeCredential();
    p.putString("panel", defaultPassword);
    p.putString("ota", otaPassword);

    if (p.getString("panel", "") != defaultPassword || p.getString("ota", "") != otaPassword)
      Serial.println("ERROR: Credentials could not be written to NVS; they will change at every startup.");
    else
      Serial.println("New credentials have been generated.");
  }
  p.end();
}

const String &apSsid() { return ssid; }
const String &panelCredential() { return defaultPassword; }
const String &otaCredential() { return otaPassword; }

void printLabel()
{
  Serial.println();
  Serial.printf ("SSID : %s\n", ssid.c_str());
  Serial.printf ("Password : %s\n", defaultPassword.c_str());
  Serial.printf ("OTA : %s\n", otaPassword.c_str());
  Serial.println("Save and store your credentials!");
}

/*
  Cihaz sıfırlanırsa modemlerdeki gibi sonradan yapılan ayarlar silinecek, orijinal şifre korunacak.
  Cihazın bir yerine modemlerdeki etiket gibi yazmanız tavsiye edilir.

  If the device is factory reset, any settings added later will be deleted, but the original password will remain.
  It is recommended that you write it somewhere on the device, similar to the label on modems.
*/

void factoryReset()
{
  Serial.println("Factory reset initiated...");
  Serial.flush();

  Preferences p;
  bool ok = true;
  for (const char *ns : { "uyanik", "uyanik-net" })
  {
    if (p.begin(ns, false)) { ok &= p.clear(); p.end(); }
    else { ok = false; Serial.printf("%s failed to open\n", ns); }
  }
  if (!ok)
  {
    Serial.println("ERROR: Cleaning couldn't be completed; the device is continuing with the previous settings.");
    lowCount = 0;
    resetRequest = false;
    return;
  }
  delay(200);
  ESP.restart();
}

void checkResetButton()
{
  if (holdNotice)
  {
    holdNotice = false;
    Serial.println("Factory reset: Hold for 5 seconds");
  }
  if (!resetRequest) return;
  factoryReset();
}

}
