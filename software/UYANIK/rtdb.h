// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

 /*
   Firebase Realtime Database REST istemcisi.
   Firebase Realtime Database REST client.
 */

class Rtdb
{
public:
  // Host geçersizse false döner istemci kalıcı olarak kapalı kalır.
  // If the host is invalid, it returns false, and the client remains permanently closed.
  bool begin(const String &apiKey, const String &dbUrl, const String &email, const String &password);

  // Uzaktan kontrolu kapatır, token'i düşürür. Ayarlar gecersiz hale gelirse cagrilmali yoksa halde istemci eski host ve eski kimlikle yazmaya devam eder.
  // Disables remote control and drops the token. If the settings become invalid, a call must be made; otherwise, the client will continue to write using the old host and old credentials.
  void stop();

  void loop();
  bool ready() const;

  bool get(const String &path, String &out);
  bool set(const String &path, const String &value);
  bool stamp(const String &path);

  const char *error() const { return _err; }

private:
  bool signIn();
  bool renew();
  int  send(const char *verb, const String &url, const char *contentType, const String &body, String &resp, size_t limit);
  bool readBody(String &out, size_t limit);
  void diagnose(const String &url);
  void backoff(const char *why);
  void gotToken(const String &token, const String &refresh, uint32_t ttlSec);
  String url(const String &path) const;
  bool   handleCode(int code);

  String _key, _host, _email, _pass;
  String _token, _refreshToken;
  uint32_t _expiry = 0, _renewAt = 0, _retryAt = 0;
  uint8_t  _fails = 0;
  bool     _enabled = false;
  const char *_err = "";

  WiFiClientSecure _tls;
  HTTPClient _http;
};

extern Rtdb rtdb;
