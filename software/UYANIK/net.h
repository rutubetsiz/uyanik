// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>
#include <IPAddress.h>

namespace net
{

void begin(const String &apSsid, const String &apPass);
void loop();

void setAp(const String &ssid, const String &pw);

bool online();
bool portalOpen();
bool apUp();
uint32_t closingIn();
String ssid();

bool fromAp(const IPAddress &localIp, const IPAddress &remoteIp);

void startScan();
bool scanning();
String scanJson();

void connect(const String &ssid, const String &password);

/*
  These should stay as they are; they shouldn't be changed. Don't change them just because they're in Turkish.
  ""  "deneniyor"  "ok"  "sifre"  "bulunamadi"  "zaman asimi"
  "ad-bos"  "ad-uzun"  "sifre-uzun"
*/

const char *result();

void forget();

}
