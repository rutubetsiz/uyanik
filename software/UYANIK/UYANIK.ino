// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

/*
  UYANIK - Uzaktan PC Güç Kontrolü

  Cihaz, bilgisayarın anakartının JFP1 header'ına bağlanır.
  PWR_SW ve RESET_SW ile komut verilir PWR_LED'den bilgisayarın açık veya kapalı oldugu okunur.


  UYANIK - Remote PC Power Control

  The device connects to the JFP1 header on the computer's motherboard.
  Commands are issued via PWR_SW and RESET_SW, and the PWR_LED indicates whether the computer is on or off.
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <esp_random.h>
#include <time.h>

#include "net.h"
#include "json.h"
#include "settings.h"
#include "identity.h"
#include "press.h"
#include "rtdb.h"
#include "panel_html.h"

// PINS
#define PIN_PWR 25
#define PIN_RST 26
#define PIN_PWR_LED 27
#define PIN_STATUS_LED 32
#define PIN_FACTORY 4

// PRESS DURATIONS ms
#define PRESS_SHORT 300
#define PRESS_LONG 6000
#define PRESS_RESET 300

// TASK INTERVALS
#define POLL_INTERVAL 2000UL
#define STATE_INTERVAL 5000UL
#define STAMP_INTERVAL 30000UL
#define IP_INTERVAL 300000UL

// POWER LED ms
#define LED_SAMPLE 20UL
#define LED_STEADY 2500UL
#define LED_BLINK  4000UL

using settings::cfg;

void startRemote();
bool timeReady();
static bool apiOk();

WebServer server(80);

static String   panelToken;
static uint8_t  loginFails = 0;
static uint32_t lockUntil  = 0;
static bool     locked     = false;

static String makeToken()
{
  char b[33];
  for (uint8_t i = 0; i < 4; i++) sprintf(b + i * 8, "%08x", (unsigned)esp_random());
  b[32] = 0;
  return String(b);
}

static const String &panelPassword()
{
  return cfg.panelPw.length() ? cfg.panelPw : identity::panelCredential();
}

static const char AP_PREFIX[] = "UYANIK-";

static String apName()
{
  return cfg.apSuffix.length() ? String(AP_PREFIX) + cfg.apSuffix : identity::apSsid();
}

static const String &apPassword()
{
  return cfg.apPw.length() ? cfg.apPw : panelPassword();
}

static String apSuffixEff()
{
  if (cfg.apSuffix.length()) return cfg.apSuffix;
  return identity::apSsid().substring(sizeof(AP_PREFIX) - 1);
}

String configError;

static SemaphoreHandle_t rMux = nullptr;
static inline void rLock() { xSemaphoreTake(rMux, portMAX_DELAY); }
static inline void rUnlock() { xSemaphoreGive(rMux); }

static volatile bool rCfgDirty = true;

static bool rReady = false;
static char rErr[48]   = "";
static char rPubIp[46] = "";

static void publish(bool ready, const char *err, const String &pubIp)
{
  rLock();
  rReady = ready;
  strncpy(rErr, err ? err : "", sizeof(rErr) - 1);   rErr[sizeof(rErr)-1] = 0;
  strncpy(rPubIp, pubIp.c_str(), sizeof(rPubIp) - 1); rPubIp[sizeof(rPubIp)-1] = 0;
  rUnlock();
}

enum PcState : uint8_t { PC_OFF, PC_ON, PC_SLEEP };

// remoteTask diğer çekirdekten okuyor. remoteTask reads these from the other core.
static volatile uint8_t pcState  = PC_OFF;
static volatile bool    pcStable = false;

static const char *pcStateName()
{
  switch (pcState)
  {
    case PC_ON:    return "on";
    // case PC_SLEEP: return "sleep";
    default:       return "off";
  }
}

static void ledBegin()
{
  pinMode(PIN_PWR_LED, INPUT_PULLUP);

  // Boot sırasında LED anlık yanmasın diye önce LOW, sonra OUTPUT.
  // LOW first, then OUTPUT, so the LED does not flash during boot.
  digitalWrite(PIN_STATUS_LED, LOW);
  pinMode(PIN_STATUS_LED, OUTPUT);
}

static void ledLoop()
{
  static uint32_t lastSample = 0, lastEdge = 0, prevEdge = 0;
  static bool lit = false, started = false, hadEdge = false;

  uint32_t now = millis();
  if (started && now - lastSample < LED_SAMPLE) return;
  lastSample = now;

  bool on = digitalRead(PIN_PWR_LED) == LOW;

  if (!started)
  {
    started = true;
    lit = on;
    lastEdge = now;
  }
  else if (on != lit)
  {
    lit = on;
    prevEdge = hadEdge ? lastEdge : 0;   // açılış anı gerçek bir değişim değil / boot is not a real change
    lastEdge = now;
    hadEdge = true;
  }

  if (now - lastEdge >= LED_STEADY)
  {
    pcState = lit ? PC_ON : PC_OFF;
    pcStable = true;
  }
  else if (prevEdge && now - prevEdge < LED_BLINK)
  {
    pcState = PC_SLEEP;
    pcStable = true;
  }

  digitalWrite(PIN_STATUS_LED, lit ? HIGH : LOW);
}

bool runCommand(const String &cmd)
{
  if (cmd == "power_short") return press::power(PRESS_SHORT);
  if (cmd == "power_long")  return press::power(PRESS_LONG);
  if (cmd == "reset")       return press::reset(PRESS_RESET);
  Serial.printf("[cmd] unknown: %s\n", cmd.c_str());
  return false;
}

String path(const char *leaf) { return "/" + cfg.deviceName + "/" + leaf; }

static bool authOk()
{
  if (panelToken.length() && server.header("X-Uyanik-Token") == panelToken) return true;

  if (server.authenticate("uyanik", panelPassword().c_str())) return true;

  server.sendHeader("Cache-Control", "no-store");
  server.send(401, "application/json", "{\"ok\":false,\"error\":\"giris gerekli\"}");
  return false;
}

void handleRoot()
{
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html; charset=utf-8", PANEL_HTML);
}

void handleStatus()
{
  if (!apiOk()) return;
  String j = "{";
  j += "\"state\":" + json::quote(pcStateName()) + ",";
  j += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  j += "\"localIp\":" + json::quote(WiFi.localIP().toString()) + ",";

  bool  snapReady;
  char  snapErr[48], snapIp[46];
  rLock();
  snapReady = rReady;
  strcpy(snapErr, rErr);
  strcpy(snapIp, rPubIp);
  rUnlock();

  j += "\"publicIp\":" + json::quote(snapIp) + ",";
  j += "\"remote\":" + json::quote(snapReady ? "ok" : snapErr) + ",";
  j += "\"configError\":" + json::quote(configError) + ",";
  j += "\"wifiSsid\":" + json::quote(net::ssid()) + ",";
  j += "\"wifiResult\":" + json::quote(net::result()) + ",";
  j += "\"portal\":" + String(net::portalOpen() ? "true" : "false") + ",";
  j += "\"apUp\":" + String(net::apUp() ? "true" : "false") + ",";
  j += "\"closingIn\":" + String(net::closingIn()) + ",";
  j += "\"apSsid\":" + json::quote(apName()) + ",";
  j += "\"pcName\":" + json::quote(cfg.pcName) + ",";
  j += "\"mdns\":" + json::quote(cfg.deviceName + ".local") + ",";
  j += "\"remoteOk\":" + String(snapReady ? "true" : "false") + ",";
  j += "\"clock\":" + String(timeReady() ? "true" : "false") + ",";
  j += "\"uptime\":" + String(millis() / 1000);
  j += "}";
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", j);
}

void handleCmd()
{
  if (!apiOk()) return;
  if (press::busy()) { server.send(409, "text/plain", "mesgul"); return; }
  if (runCommand(server.arg("c"))) server.send(200, "text/plain", "ok");
  else server.send(400, "text/plain", "gecersiz komut");
}

static String bodyField(const String &body, const char *key)
{
  String pat = String("\"") + key + "\"";
  int k = body.indexOf(pat); if (k < 0) return "";
  int c = body.indexOf(':', k + pat.length()); if (c < 0) return "";
  int a = body.indexOf('"', c); if (a < 0) return "";

  String raw;
  unsigned i = a + 1;
  while (i < body.length())
  {
    char ch = body[i];
    if (ch == '\\' && i + 1 < body.length()) { raw += ch; raw += body[i + 1]; i += 2; continue; }
    if (ch == '"') break;
    raw += ch;
    i++;
  }
  return json::unquote(raw);
}

static bool crossSiteOk()
{
  if (server.header("X-Uyanik") == "1") return true;
  server.send(403, "text/plain", "capraz kaynak istegi");
  return false;
}

static bool hostOk()
{

  if (net::fromAp(server.client().localIP(), server.client().remoteIP()))
  return true;

  String h = server.hostHeader();
  int c = h.indexOf(':');
  if (c >= 0) h = h.substring(0, c);
  if (!h.length()) return true;
  h.toLowerCase();

  bool ipLiteral = true;
  for (unsigned i = 0; i < h.length(); i++)
  if ((h[i] < '0' || h[i] > '9') && h[i] != '.') { ipLiteral = false; break; }
  if (ipLiteral || h.endsWith(".local")) return true;

  Serial.printf("[web] unexpected Host rejected: %s\n", h.c_str());
  server.send(403, "text/plain", "beklenmeyen Host");
  return false;
}

static bool apiOk()
{
  return authOk() && hostOk() && crossSiteOk();
}

void handleLogin()
{
  if (!hostOk() || !crossSiteOk()) return;

  uint32_t now = millis();

  if (locked && (int32_t)(now - lockUntil) < 0)
  {
    server.send(429, "application/json", "{\"ok\":false,\"error\":\"kilitli\",\"wait\":" + String((lockUntil - now) / 1000 + 1) + "}");
    return;
  }
  locked = false;

  String pass = bodyField(server.arg("plain"), "pass");
  if (pass.length() && pass == panelPassword())
  {
    loginFails = 0;
    if (!panelToken.length()) panelToken = makeToken();
    server.sendHeader("Cache-Control", "no-store");
    server.send(200, "application/json", "{\"ok\":true,\"token\":" + json::quote(panelToken) + "}");
    return;
  }

  if (++loginFails >= 5){ loginFails = 0; locked = true; lockUntil = millis() + 30000; }
  Serial.println("[web] Panel login denied");
  server.send(401, "application/json", "{\"ok\":false,\"error\":\"parola yanlis\"}");
}

void handlePanelPw()
{
  if (!apiOk()) return;
  String b = server.arg("plain");

  if (bodyField(b, "current") != panelPassword())
  {
    server.send(200, "application/json", "{\"ok\":false,\"error\":\"mevcut parola yanlis\"}");
    return;
  }

  String next = bodyField(b, "next");
  const char *bad = settings::checkPanelPw(next);
  if (bad)
  {
    server.send(200, "application/json", "{\"ok\":false,\"error\":" + json::quote(bad) + "}");
    return;
  }

  String prev = cfg.panelPw;
  cfg.panelPw = next;
  if (!settings::save())
  {
    cfg.panelPw = prev;
    server.send(200, "application/json", "{\"ok\":false,\"error\":\"ayarlar kaydedilemedi\"}");
    return;
  }

  net::setAp(apName(), apPassword());

  panelToken = makeToken();
  Serial.println("[web] panel password changed");
  server.send(200, "application/json", "{\"ok\":true,\"token\":" + json::quote(panelToken) + "}");
}

void handleRestart()
{
  if (!apiOk()) return;
  server.send(200, "application/json", "{\"ok\":true}");
  server.client().flush();
  delay(200);
  ESP.restart();
}

void handleLogout()
{
  if (!apiOk()) return;
  panelToken = "";
  server.send(200, "application/json", "{\"ok\":true}");
}

void handleScanGet()
{
  if (!apiOk()) return;
  String j = net::scanning() ? "{\"scanning\":true}" : "{\"scanning\":false,\"nets\":" + net::scanJson() + "}";
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", j);
}

void handleScanStart()
{
  if (!apiOk()) return;
  net::startScan();
  server.send(200, "application/json", "{\"ok\":true}");
}

void handleWifiConnect()
{
  if (!apiOk()) return;
  String b = server.arg("plain");
  String ssid = bodyField(b, "ssid");
  server.send(200, "application/json", "{\"ok\":true}");
  server.client().flush();
  net::connect(ssid, bodyField(b, "pass"));
}

void handleWifiForget()
{
  if (!apiOk()) return;
  server.send(200, "application/json", "{\"ok\":true}");
  server.client().flush();
  net::forget();
}

void handleFactory()
{
  if (!apiOk()) return;
  server.send(200, "application/json", "{\"ok\":true}");
  server.client().flush();
  delay(200);
  identity::factoryReset();
}

void handleSettingsGet()
{
  if (!apiOk()) return;
  String j = "{";
  j += "\"deviceName\":" + json::quote(cfg.deviceName) + ",";
  j += "\"apiKey\":" + json::quote(cfg.apiKey) + ",";
  j += "\"dbUrl\":" + json::quote(cfg.dbUrl) + ",";
  j += "\"email\":" + json::quote(cfg.email) + ",";
  j += "\"pcName\":" + json::quote(cfg.pcName) + ",";
  j += "\"apSuffix\":" + json::quote(apSuffixEff()) + ",";
  j += "\"error\":" + json::quote(configError);
  j += "}";
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", j);
}

void handleSettingsPost()
{
  if (!apiOk()) return;
  String b = server.arg("plain");
  settings::Config in = cfg;
  in.deviceName = bodyField(b, "deviceName");
  in.apiKey = bodyField(b, "apiKey");
  in.dbUrl = bodyField(b, "dbUrl");
  in.email = bodyField(b, "email");
  in.password = bodyField(b, "password");
  in.pcName = bodyField(b, "pcName");
  in.apSuffix = bodyField(b, "apSuffix");
  in.apPw = bodyField(b, "apPw");

  const char *bad = applySettings(in);
  if (bad)
  {
    configError = bad;
    server.send(200, "application/json", "{\"ok\":false,\"error\":" + json::quote(bad) + "}");
    return;
  }

  server.send(200, "application/json", "{\"ok\":true}");
  startRemote();
}

void setupWebServer()
{
  static const char *kept[] = { "X-Uyanik", "X-Uyanik-Token" };
  server.collectHeaders(kept, 2);

  server.on("/", HTTP_GET,  handleRoot);
  server.on("/api/status", HTTP_GET,  handleStatus);
  server.on("/api/cmd", HTTP_POST, handleCmd);
  server.on("/api/settings", HTTP_GET,  handleSettingsGet);
  server.on("/api/settings", HTTP_POST, handleSettingsPost);
  server.on("/api/wifi/scan", HTTP_GET,  handleScanGet);
  server.on("/api/wifi/scan", HTTP_POST, handleScanStart);
  server.on("/api/wifi/connect", HTTP_POST, handleWifiConnect);
  server.on("/api/wifi/forget", HTTP_POST, handleWifiForget);
  server.on("/api/factory", HTTP_POST, handleFactory);
  server.on("/api/login", HTTP_POST, handleLogin);
  server.on("/api/logout", HTTP_POST, handleLogout);
  server.on("/api/panelpw", HTTP_POST, handlePanelPw);
  server.on("/api/restart", HTTP_POST, handleRestart);

  server.onNotFound([]()
  {
    if (!net::fromAp(server.client().localIP(), server.client().remoteIP()))
    {
      server.send(404, "text/plain", "yok");
      return;
    }
    server.sendHeader("Cache-Control", "no-store");
    server.send_P(200, "text/html; charset=utf-8", PANEL_HTML);
  });
  server.begin();
}

//  SETUP PORTAL

static bool ntpStarted = false;

bool timeReady(){ return time(nullptr) > 1700000000; }   // 2023-11 sonrasi

void startNtp()
{
  if (ntpStarted) return;
  ntpStarted = true;
  configTime(0, 0, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
  Serial.println("[time] NTP started");
}

const char *applySettings(settings::Config in)
{

  if (!in.password.length()) in.password = cfg.password;
  if (!in.apPw.length()) in.apPw = cfg.apPw;

  const char *bad = nullptr;
  String host;
  if (in.dbUrl.length()) bad = settings::checkDbUrl(in.dbUrl, host);
  if (!bad) bad = settings::checkDeviceName(in.deviceName);
  if (!bad) bad = settings::checkPcName(in.pcName);
  if (!bad) bad = settings::checkApSuffix(in.apSuffix);
  if (!bad && in.apPw.length()) bad = settings::checkApPw(in.apPw);
  if (bad) return bad;

  cfg = in;
  if (!settings::save()) return "ayarlar kaydedilemedi";

  net::setAp(apName(), apPassword());

  configError = "";
  return nullptr;
}

void startRemote()
{
  rLock();
  rCfgDirty = true;
  rUnlock();
  if (!cfg.complete()) {
    configError = "Firebase ayarlari eksik";
    Serial.println("[rtdb] Settings are missing; remote control is turned off");
  }
}

static String rDev, rPcName, rPubIpLocal, rLastState;
static bool rUp = false, rAnnounced = false;

static String rPath(const char *leaf) { return "/" + rDev + "/" + leaf; }

static void remoteApplyConfig()
{
  String k, u, e, p;
  bool complete;

  rLock();
  rCfgDirty = false;
  rDev = cfg.deviceName;
  rPcName = cfg.pcName;
  k = cfg.apiKey; u = cfg.dbUrl; e = cfg.email; p = cfg.password;
  complete = cfg.complete();
  rUnlock();

  rtdb.stop();
  rAnnounced = false;
  rLastState = "";
  rUp = complete && rtdb.begin(k, u, e, p);
  publish(false, rUp ? "" : (complete ? rtdb.error() : "ayarlar eksik"), rPubIpLocal);
}

static void remotePollCommand()
{
  String cmd;
  if (!rtdb.get(rPath("cmd"), cmd)) return;
  if (cmd.length() == 0 || cmd == "none") return;
  if (press::busy()) return;

  if (!rtdb.set(rPath("cmd"), "none"))
  {
    Serial.println("[rtdb] Command could not be processed; it is not running");
    return;
  }
  Serial.printf("[rtdb] command: %s\n", cmd.c_str());
  if (!runCommand(cmd))
    Serial.printf("[rtdb] consumed but could not be executed: %s\n", cmd.c_str());
}

static void remoteReportState(uint32_t &lastStamp)
{
  // Açılıştan sonra durum oturmadan yazma. Don't report before the state settles after boot.
  if (pcStable)
  {
    String st = pcStateName();
    if (st != rLastState && rtdb.set(rPath("state"), st))
    {
      rLastState = st;
      Serial.printf("[state] %s\n", st.c_str());
    }
  }
  if (millis() - lastStamp >= STAMP_INTERVAL)
  {
    lastStamp = millis();
    rtdb.stamp(rPath("lastSeen"));
  }
}

static void remoteFetchIp()
{
  HTTPClient http;
  http.setTimeout(5000);
  if (!http.begin("http://api.ipify.org")) return;

  if (http.GET() == HTTP_CODE_OK && http.getSize() > 0 && http.getSize() < 64)
  {
    String ip = http.getString();
    ip.trim();

    if (settings::checkIp(ip) == nullptr && ip.length()) {
      rPubIpLocal = ip;
      publish(rtdb.ready(), rtdb.ready() ? "" : rtdb.error(), rPubIpLocal);
      if (rtdb.ready())
      {
        rtdb.set(rPath("ip"), rPubIpLocal);
        rtdb.set(rPath("localIp"), WiFi.localIP().toString());
      }
      Serial.printf("[ip] %s\n", rPubIpLocal.c_str());
    }
  }
  http.end();
}

static void remoteTask(void *)
{
  uint32_t lastPoll = 0, lastState = 0, lastStamp = 0;
  uint32_t lastIp = millis() - IP_INTERVAL;
  bool announcedReady = false;

  for (;;)
  {
    if (!net::online()) { vTaskDelay(pdMS_TO_TICKS(500)); continue; }

    if (rCfgDirty) remoteApplyConfig();

    uint32_t now = millis();
    if (now - lastIp >= IP_INTERVAL) { lastIp = now; remoteFetchIp(); }

    if (!rUp) { vTaskDelay(pdMS_TO_TICKS(500)); continue; }

    if (!timeReady()) { vTaskDelay(pdMS_TO_TICKS(1000)); continue; }

    rtdb.loop();

    bool ready = rtdb.ready();
    if (ready != announcedReady)
    {
      announcedReady = ready;
      publish(ready, ready ? "" : rtdb.error(), rPubIpLocal);
    }
    if (!ready) { vTaskDelay(pdMS_TO_TICKS(500)); continue; }

    if (!rAnnounced)
    {
      rAnnounced = true;
      Serial.println("[rtdb] ready");
      rtdb.set(rPath("pcName"), rPcName);
    }

    if (now - lastPoll  >= POLL_INTERVAL)  { lastPoll = now;  remotePollCommand(); }
    if (now - lastState >= STATE_INTERVAL) { lastState = now; remoteReportState(lastStamp); }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(400);
  Serial.println("\n## UYANIK ##");

  press::begin(PIN_PWR, PIN_RST);
  ledBegin();

  settings::load();
  {
    String host;
    const char *bad = nullptr;
    if (cfg.dbUrl.length()) bad = settings::checkDbUrl(cfg.dbUrl, host);
    if (!bad) bad = settings::checkDeviceName(cfg.deviceName);
    if (bad) { configError = bad; Serial.printf("[cfg] Saved setting is invalid: %s\n", bad); }
  }

  WiFi.mode(WIFI_STA);
  //WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  identity::begin(PIN_FACTORY);
  identity::printLabel();

  if (cfg.panelPw.length())
  {
    Serial.println("The panel password has been changed!");
    Serial.println("Factory reset will restore the original password.");
    Serial.println();
  }

  net::begin(apName(), apPassword());

  if (MDNS.begin(cfg.deviceName.c_str()))
    Serial.printf("[mdns] http://%s.local\n", cfg.deviceName.c_str());

  ArduinoOTA.setHostname(cfg.deviceName.c_str());
  ArduinoOTA.setPassword(identity::otaCredential().c_str());
  ArduinoOTA.begin();

  setupWebServer();

  rMux = xSemaphoreCreateMutex();
  startRemote();

  xTaskCreatePinnedToCore(remoteTask, "uyanik-rtdb", 8192, nullptr, 1, nullptr, 0);
}

void loop()
{
  ledLoop();

  if (press::takeRelease()) Serial.println("[press] released");

  identity::checkResetButton();
  server.handleClient();
  ArduinoOTA.handle();

  net::loop();
  if (!net::online()) return;

  startNtp();

  delay(1);
}
