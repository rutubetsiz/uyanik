// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#include "net.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <Preferences.h>
#include "json.h"

/*
  Ağları tarama, internete bağlanma, hotspot açıp kapatma, ağa bağlanınca kurulum ekranı.

  Scanning for networks, connecting to the internet, turning the hotspot on and off, and the setup screen when connecting to a network.
*/

namespace net
{

static const uint32_t CONNECT_TIMEOUT = 20000;
static const uint32_t HOTSPOT_FALLBACK    = 180000;
static const uint32_t RETRY_PORTAL = 60000; // İnternet geldi mi diye tekrar yoklama süresi. Check again to see if the internet is back
static const uint32_t CLOSE_GRACE     = 25000; // Kurulum sonrası hotspot'un kapanması için bekleme süresi. Grace period for the hotspot to close after setup.

// Cihaz ile evdeki modem aynı IP kullanıp çakışmasın diye farklı adres.
// Different address so the device and the home modem don't use the same IP and cause a conflict.
static const IPAddress AP_IP(192, 168, 4, 1);

static DNSServer dns;

enum State 
{
  CONNECTING,
  ONLINE,
  PORTAL
};

static volatile State st = CONNECTING;
static String apName, apPassword;
static String curSsid, curPass;
static String tryS, tryP, prevS, prevP;
static bool trying = false;
static const char *res = "";
static uint32_t attemptAt = 0, offlineSince = 0, lastRetry = 0;
static bool scanRunning = false;
static uint32_t closeAt = 0;

// NVS

static void loadCreds()
{
  Preferences p;
  p.begin("uyanik-net", true);
  curSsid = p.getString("ssid", "");
  curPass = p.getString("pass", "");
  p.end();
}

static void saveCreds(const String &s, const String &pw)
{
  Preferences p;
  p.begin("uyanik-net", false);
  p.putString("ssid", s);
  p.putString("pass", pw);
  p.end();
  curSsid = s;
  curPass = pw;
}

// AP

static void openPortal()
{
  if (st == PORTAL) return;
  st = PORTAL;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apName.c_str(), apPassword.c_str());
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", AP_IP);
  Serial.printf("The portal has opened: %s (http://%s)\n",
  apName.c_str(), AP_IP.toString().c_str());
}

static void closePortal()
{
  closeAt = 0;
  dns.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.println("The hotspot is off");
}

// BAĞLANTI

static void beginAttempt(const String &s, const String &pw)
{
  Serial.printf("Connecting: %s\n", s.c_str());

  if (scanRunning) { WiFi.scanDelete(); scanRunning = false; }

  WiFi.disconnect(false, false);
  delay(150);

  WiFi.begin(s.c_str(), pw.c_str());
  attemptAt = millis();
  if (st != PORTAL) st = CONNECTING;
}

void setAp(const String &ssid, const String &pw)
{
  if (apName == ssid && apPassword == pw) return;
  apName = ssid;
  apPassword = pw;

  if (apUp())
  {
    WiFi.softAP(apName.c_str(), apPassword.c_str());
    Serial.print("The hotspot has been updated: ");
    Serial.println(apName);
  }
}

void begin(const String &ap, const String &pw)
{
  apName = ap;
  apPassword = pw;
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  loadCreds();
  offlineSince = millis();

  if (curSsid.length()) beginAttempt(curSsid, curPass);
  else openPortal();
}

static const char *failReason()
{
  switch (WiFi.status()) {
    case WL_NO_SSID_AVAIL:   return "bulunamadi";
    case WL_CONNECT_FAILED:  return "sifre";
    default: return "zaman asimi";
  }
}

static void attemptFailed()
{
  res = failReason();
  Serial.printf("Connection failed: %s\n", res);

  if (trying)
  {
    // Eskisine geri dön ki cihaz yanlış şifre yüzünden ağsız kalmasın. Return to the previous settings so the device doesn't lose its network connection due to an incorrect password.
    trying = false;
    curSsid = prevS;
    curPass = prevP;
    if (curSsid.length()) Serial.println("Previous network credentials restored");
    if (st != PORTAL) openPortal();
    lastRetry = millis();
    attemptAt = 0;
    return;
  }
  if (curSsid.length()) beginAttempt(curSsid, curPass);
  else { openPortal(); attemptAt = 0; }
}

void loop()
{
  // AP açıkken DNS işlenmeye devam etmeli yoksa otomatik kurulum ekranına yönlendirme çalışmaz. DNS must continue to process while AP is open; otherwise, the redirection to the automatic setup screen will not work.
  if (apUp()) dns.processNextRequest();

  bool connected = WiFi.status() == WL_CONNECTED;

  if (connected)
  {
    if (st != ONLINE)
    {
      bool wasPortal = (st == PORTAL);
      st = ONLINE;
      offlineSince = 0;
      res = "ok";
      if (trying)
      {
        trying = false;
        saveCreds(tryS, tryP);
        Serial.println("New network saved");
      }
      Serial.printf("Connected: %s  %s\n",
                    WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

      if (wasPortal)
      {
        closeAt = millis() + CLOSE_GRACE;
        Serial.printf("The hotspot will close in %lu seconds\n",
        (unsigned long)(CLOSE_GRACE / 1000));
      }
    }
    if (closeAt && (int32_t)(millis() - closeAt) >= 0) closePortal();
    return;
  }

  // BAĞLI DEĞİL | NOT CONNECTED
  if (st == ONLINE)
  {
    offlineSince = millis();
    Serial.println("Connection lost");

    if (closeAt)
    {
      closeAt = 0;
      st = PORTAL;
      Serial.println("Cancellation of closure, the hotspot remains open");
    } 
    else 
    {
      st = CONNECTING;
    }
    beginAttempt(curSsid, curPass);
    return;
  }
  if (!offlineSince) offlineSince = millis();

  if (attemptAt && millis() - attemptAt <= CONNECT_TIMEOUT) return;

  if (attemptAt)
  {
    attemptAt = 0;
    if (!trying && st != PORTAL && millis() - offlineSince > HOTSPOT_FALLBACK)
    {
      res = failReason();
      openPortal();
      lastRetry = millis();
      return;
    }
    attemptFailed();
    return;
  }

  if (st == PORTAL && !trying && curSsid.length() && millis() - lastRetry > RETRY_PORTAL) 
  {
    lastRetry = millis();
    beginAttempt(curSsid, curPass);
  }
}

bool online() { return st == ONLINE; }
bool portalOpen() { return st == PORTAL; }
bool apUp() { return st == PORTAL || closeAt != 0; }

uint32_t closingIn() 
{
  if (!closeAt) return 0;
  int32_t left = (int32_t)(closeAt - millis());
  return left > 0 ? (uint32_t)(left / 1000) + 1 : 0;
}

String ssid() { return curSsid; }
const char *result() { return res; }

bool fromAp(const IPAddress &localIp, const IPAddress &remoteIp)
{
  if (!apUp()) return false;
  if (localIp == AP_IP) return true;
  if ((uint32_t)localIp != 0) return false;
  return remoteIp[0] == AP_IP[0] && remoteIp[1] == AP_IP[1] && remoteIp[2] == AP_IP[2];
}

// SCANING

void startScan()
{
  if (scanRunning) return;
  WiFi.scanDelete();
  WiFi.scanNetworks(true, false);
  scanRunning = true;
}

bool scanning()
{
  if (!scanRunning) return false;
  if (WiFi.scanComplete() == WIFI_SCAN_RUNNING) return true;
  scanRunning = false;
  return false;
}

// !
static String sanitizeUtf8(const String &s)
{
  String o;
  o.reserve(s.length());
  unsigned i = 0, n = s.length();
  while (i < n) {
    uint8_t c = (uint8_t)s[i];
    uint8_t need = c < 0x80 ? 0 : (c >= 0xC2 && c <= 0xDF) ? 1
                                : (c >= 0xE0 && c <= 0xEF) ? 2
                                : (c >= 0xF0 && c <= 0xF4) ? 3 : 255;
    if (need == 255) { o += '?'; i++; continue; }
    if (i + need >= n) { o += '?'; i++; continue; }
    bool ok = true;
    for (uint8_t k = 1; k <= need; k++)
      if (((uint8_t)s[i + k] & 0xC0) != 0x80) { ok = false; break; }
    if (!ok) { o += '?'; i++; continue; }
    for (uint8_t k = 0; k <= need; k++) o += s[i + k];
    i += need + 1;
  }
  return o;
}

String scanJson()
{
  int n = WiFi.scanComplete();
  if (n == WIFI_SCAN_RUNNING || n == WIFI_SCAN_FAILED) return "[]";

  String j = "[";
  int shown = 0;
  for (int i = 0; i < n && shown < 20; i++)
  {
    if (WiFi.SSID(i).length() == 0) continue;
    if (shown) j += ",";
    j += "{\"s\":" + json::quote(sanitizeUtf8(WiFi.SSID(i)));
    j += ",\"r\":" + String(WiFi.RSSI(i));
    j += ",\"e\":" + String(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? 0 : 1);
    j += "}";
    shown++;
  }
  return j + "]";
}

void connect(const String &s, const String &pw)
{
  if (!s.length()) { res = "ad-bos"; return; }
  if (s.length() > 32)  { res = "ad-uzun";    Serial.println("SSID > 32");  return; }
  if (pw.length() > 64) { res = "sifre-uzun"; Serial.println("sifre > 64"); return; }

  prevS = curSsid;
  prevP = curPass;
  tryS = s;
  tryP = pw;
  trying = true;
  res = "deneniyor";
  offlineSince = millis();
  beginAttempt(s, pw);
}

void forget()
{
  Serial.println("Network settings are removed");
  Preferences p;
  p.begin("uyanik-net", false);
  p.clear();
  p.end();
  curSsid = ""; curPass = "";
  trying = false;
  res = "";
  closeAt = 0;
  WiFi.disconnect(false, false);
  st = CONNECTING;
  openPortal();
}

}
