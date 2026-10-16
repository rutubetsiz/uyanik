// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rtdb.h"
#include "gts_root.h"
#include "settings.h"
#include "json.h"
#include <WiFi.h>
#include <esp_random.h>
#include <time.h>

Rtdb rtdb;

static const char *AUTH_URL = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=";
static const char *RENEW_URL = "https://securetoken.googleapis.com/v1/token?key=";

static const uint32_t RENEW_MARGIN = 300000;
static const uint32_t TIMEOUT = 8000;
static const size_t AUTH_LIMIT = 4096;
static const size_t DATA_LIMIT = 1024;

static inline bool due(uint32_t t) { return (int32_t)(millis() - t) >= 0; }

static bool field(const String &src, const char *key, String &out)
{
  String pat = String("\"") + key + "\"";
  int k = src.indexOf(pat); if (k < 0) return false;
  int c = src.indexOf(':', k + pat.length()); if (c < 0) return false;
  int a = src.indexOf('"', c); if (a < 0) return false;
  int b = src.indexOf('"', a + 1); if (b < 0) return false;
  out = src.substring(a + 1, b);
  return true;
}

static String urlEncode(const String &s)
{
  String o;
  o.reserve(s.length() + 8);
  char hex[4];
  for (unsigned i = 0; i < s.length(); i++) {
    char c = s[i];
    bool safe = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') || c == '-' || c == '_' ||
                c == '.' || c == '~';
    if (safe) { o += c; }
    else { snprintf(hex, sizeof(hex), "%%%02X", (uint8_t)c); o += hex; }
  }
  return o;
}

// SETUP

bool Rtdb::begin(const String &apiKey, const String &dbUrl, const String &email, const String &password)
{
  _enabled = false;
  _token = ""; _refreshToken = "";

  const char *bad = settings::checkDbUrl(dbUrl, _host);
  if (bad)
  {
    _err = bad;
    Serial.printf("[rtdb] Address rejected: %s\n", bad);
    return false; 
  }
  if (!apiKey.length() || !email.length() || !password.length())
  {
    _err = "kimlik bilgileri eksik";
    return false;
  }

  _key = apiKey; _email = email; _pass = password;

  _tls.setCACert(GTS_ROOTS);
  _tls.setTimeout(TIMEOUT / 1000);
  _tls.setHandshakeTimeout(TIMEOUT / 1000);
  _http.setReuse(true);
  _http.setTimeout(TIMEOUT);

  _retryAt = millis();
  _fails = 0;
  _err = "";
  _enabled = true;
  Serial.printf("[rtdb] host: %s\n", _host.c_str());
  return true;
}

void Rtdb::stop()
{
  _enabled = false;
  _token = "";
  _refreshToken = "";
}

bool Rtdb::ready() const
{
  return _enabled && _token.length() > 0 && !due(_expiry);
}

// HTTP

class BoundedSink : public Stream
{
public:
  BoundedSink(String &out, size_t limit) : _out(out), _limit(limit) {}
  size_t write(uint8_t c) override
  {
    if (_out.length() >= _limit) { _over = true; return 0; }
    _out += (char)c;
    return 1;
  }
  size_t write(const uint8_t *buf, size_t n) override
  {
    if (_out.length() + n > _limit) { _over = true; return 0; }
    _out.concat((const char *)buf, n);
    return n;
  }
  int  available() override { return 0; }
  int  read() override      { return -1; }
  int  peek() override      { return -1; }
  void flush() override     {}
  bool over() const         { return _over; }
private:
  String &_out;
  size_t  _limit;
  bool    _over = false;
};

bool Rtdb::readBody(String &out, size_t limit)
{
  int len = _http.getSize();
  if (len > (int)limit)
  {
    _err = "cevap cok buyuk";
    Serial.printf("[rtdb] response %d bytes, limit %u - discarding\n", len, (unsigned)limit);
    return false;
  }

  out = "";
  out.reserve(len > 0 ? (size_t)len : 128);

  BoundedSink sink(out, limit);
  int n = _http.writeToStream(&sink);

  if (sink.over())
  {
    _err = "cevap cok buyuk";
    Serial.println("[rtdb] response exceeds limit - discarding");
    return false;
  }

  if (n < 0)
  {
    _err = "govde okunamadi";
    Serial.printf("[rtdb] response could not be read (%d)\n", n);
    return false;
  }
  return true;
}

void Rtdb::diagnose(const String &u) {
  int a = u.indexOf("//") + 2;
  int b = -1;
  for (unsigned i = a; i < u.length(); i++)
  {
    char c = u[i];
    if (c == '/' || c == '?' || c == '#') { b = i; break; }
  }
  String host = u.substring(a, b < 0 ? u.length() : b);

  time_t t = time(nullptr);
  struct tm tmv;
  gmtime_r(&t, &tmv);
  Serial.printf("[rtdb] diag: host=%s wifi=%d heap=%u time=%04d-%02d-%02d %02d:%02d UTC\n", 
  host.c_str(), (int)WiFi.status(), (unsigned)ESP.getFreeHeap(), tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday, tmv.tm_hour, tmv.tm_min);
  
  if (t < 1700000000) Serial.println("[rtdb] diag: clock not set (date validation unconfirmed)");

  IPAddress ip;
  if (!WiFi.hostByName(host.c_str(), ip)) 
  {
    Serial.println("[rtdb] diag: DNS resolution failed");
    return;
  }
  Serial.printf("[rtdb] diag: DNS ok -> %s\n", ip.toString().c_str());

  char err[128] = {0};
  int e = _tls.lastError(err, sizeof(err));
  if (e) Serial.printf("[rtdb] diag: TLS error %d %s\n", e, err);
  else Serial.println("[rtdb] diag: no TLS error reported (port 443 might be blocked)");
}

int Rtdb::send(const char *verb, const String &u, const char *contentType, const String &body, String &resp, size_t limit) 
{
  if (!_http.begin(_tls, u)) { Serial.println("[rtdb] URL could not be resolved"); return -1; }
  if (contentType) _http.addHeader("Content-Type", contentType);

  int code = (strcmp(verb, "GET") == 0) ? _http.GET() : _http.sendRequest(verb, (uint8_t *)body.c_str(), body.length());

  if (code > 0 && !readBody(resp, limit)) code = -2;
  _http.end();

  if (code < 0 && code != -2) {
    static bool ran = false;
    static uint32_t lastAt = 0;
    if (!ran || millis() - lastAt > 60000) { ran = true; lastAt = millis(); diagnose(u); }
  }
  return code;
}

String Rtdb::url(const String &path) const
{
  return "https://" + _host + path + ".json?auth=" + _token;
}

bool Rtdb::handleCode(int code)
{
  if (code == 401 || code == 403)
  {
    _token = "";
    _err = "yetki reddedildi";
    return false;
  }
  return code == 200;
}

// TOKEN

void Rtdb::backoff(const char *why)
{
  _err = why;
  static const uint32_t steps[] = { 5000, 15000, 60000, 300000, 900000 };
  const uint8_t n = sizeof(steps) / sizeof(steps[0]);
  uint32_t wait = steps[_fails < n ? _fails : n - 1];
  if (_fails < n) _fails++;

  wait = wait * 8 / 10 + esp_random() % (wait * 4 / 10 + 1);

  _retryAt = millis() + wait;
  Serial.printf("[rtdb] %s, retrying in %lu s\n", why, (unsigned long)(wait / 1000));
}

void Rtdb::gotToken(const String &token, const String &refresh, uint32_t ttlSec)
{
  _token = token;
  if (refresh.length()) _refreshToken = refresh;
  if (ttlSec < 600) ttlSec = 600;
  _expiry  = millis() + ttlSec * 1000;
  _renewAt = _expiry - RENEW_MARGIN;
  _fails = 0;
  _err = "";
  Serial.printf("[rtdb] token received, valid for %lu min\n", (unsigned long)(ttlSec / 60));
}

bool Rtdb::signIn()
{
  String body = String("{\"email\":") + json::quote(_email) + ",\"password\":" + json::quote(_pass) + ",\"returnSecureToken\":true}";
  String resp;
  int code = send("POST", String(AUTH_URL) + _key, "application/json", body, resp, AUTH_LIMIT);
  if (code != 200)
  {
    String msg;
    field(resp, "message", msg);
    Serial.printf("[rtdb] login denied (%d) %s\n", code, msg.c_str());
    return false;
  }
  String token, refresh, ttl;
  if (!field(resp, "idToken", token)) return false;
  field(resp, "refreshToken", refresh);
  field(resp, "expiresIn", ttl);
  gotToken(token, refresh, ttl.toInt());
  return true;
}

bool Rtdb::renew()
{
  if (!_refreshToken.length()) return false;
  String body = "grant_type=refresh_token&refresh_token=" + urlEncode(_refreshToken);
  String resp;
  int code = send("POST", String(RENEW_URL) + _key, "application/x-www-form-urlencoded", body, resp, AUTH_LIMIT);
  if (code != 200) return false;
  String token, refresh, ttl;
  if (!field(resp, "id_token", token)) return false;
  field(resp, "refresh_token", refresh);
  field(resp, "expires_in", ttl);
  gotToken(token, refresh, ttl.toInt());
  return true;
}

void Rtdb::loop()
{
  if (!_enabled || WiFi.status() != WL_CONNECTED) return;
  if (!due(_retryAt)) return;

  if (!_token.length())
  {
    if (!signIn()) backoff("giris basarisiz");
    return;
  }
  if (due(_renewAt))
  {
    if (renew()) return;
    Serial.println("[rtdb] token refresh failed, signing in from scratch");
    _token = "";
    if (!signIn()) backoff("giris basarisiz");
  }
}

// DATA

bool Rtdb::get(const String &path, String &out)
{
  if (!ready()) return false;
  if (!settings::validPath(path)) { _err = "gecersiz yol"; return false; }

  String resp;
  int code = send("GET", url(path), nullptr, "", resp, DATA_LIMIT);
  if (code == 404) { out = ""; return true; }
  if (!handleCode(code))
  {
    if (code != 401 && code != 403) _err = "okuma hatasi";
    return false;
  }

  resp.trim();
  if (resp == "null") { out = ""; return true; }
  if (resp.length() >= 2 && resp[0] == '"' && resp[resp.length() - 1] == '"')
    out = json::unquote(resp.substring(1, resp.length() - 1));
  else
    out = resp;
  return true;
}

bool Rtdb::set(const String &path, const String &value)
{
  if (!ready()) return false;
  if (!settings::validPath(path)) { _err = "gecersiz yol"; return false; }

  String resp;
  int code = send("PUT", url(path), "application/json", json::quote(value), resp, DATA_LIMIT);
  if (!handleCode(code))
  {
    if (code != 401 && code != 403) _err = "yazma hatasi";
    return false;
  }
  return true;
}

bool Rtdb::stamp(const String &path)
{
  if (!ready()) return false;
  if (!settings::validPath(path)) { _err = "gecersiz yol"; return false; }

  String resp;
  int code = send("PUT", url(path), "application/json", "{\".sv\":\"timestamp\"}", resp, DATA_LIMIT);
  if (!handleCode(code))
  {
    if (code != 401 && code != 403) _err = "zaman damgasi yazilamadi";
    return false;
  }
  return true;
}
