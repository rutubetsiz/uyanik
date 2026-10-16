// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

namespace settings
{

struct Config
{
  String apiKey, dbUrl, email, password;
  String deviceName = "uyanik";

  // Kullanıcının panelden verdiği parola.
  // The password the user entered in the panel.
  String panelPw;

  // Cihaz sekmesinde görünen ad.
  // The name displayed on the Device tab.
  String pcName;

  // Hotspot adinin "UYANIK-" sonrasi.
  // The part of the hotspot name after “UYANIK-”.
  String apSuffix;

  // Hotspot WPA2 parolasi.
  // Hotspot WPA2 password.
  String apPw;

  bool complete() const
  {
    return apiKey.length() && dbUrl.length() && email.length() && password.length();
  }
};

extern Config cfg;

void load();
bool save();

const char *checkDbUrl(const String &in, String &out);

const char *checkDeviceName(const String &in);

const char *checkPanelPw(const String &in);

const char *checkPcName(const String &in);

const char *checkApSuffix(const String &in);

const char *checkApPw(const String &in);

bool validPath(const String &path);

const char *checkIp(const String &in);

}
