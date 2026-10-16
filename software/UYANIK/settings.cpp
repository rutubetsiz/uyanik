// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.h"
#include <Preferences.h>
#include <string.h>

namespace settings
{

Config cfg;

void load()
{
  Preferences p;
  p.begin("uyanik", true);
  cfg.apiKey = p.getString("apikey", "");
  cfg.dbUrl = p.getString("dburl", "");
  cfg.email = p.getString("email", "");
  cfg.password = p.getString("pass", "");
  cfg.deviceName = p.getString("name", "uyanik");
  cfg.panelPw = p.getString("panelpw", "");
  cfg.pcName = p.getString("pcname", "");
  cfg.apSuffix = p.getString("apsuffix", "");
  cfg.apPw = p.getString("appw", "");
  p.end();
}

bool save()
{
  Preferences p;
  if (!p.begin("uyanik", false))
  {
    Serial.println("[cfg] ERROR: NVS could not be opened");
    return false;
  }
  bool ok = true;
  ok &= p.putString("apikey", cfg.apiKey) == cfg.apiKey.length();
  ok &= p.putString("dburl", cfg.dbUrl) == cfg.dbUrl.length();
  ok &= p.putString("email", cfg.email) == cfg.email.length();
  ok &= p.putString("pass", cfg.password) == cfg.password.length();
  ok &= p.putString("name", cfg.deviceName) == cfg.deviceName.length();
  ok &= p.putString("panelpw", cfg.panelPw) == cfg.panelPw.length();
  ok &= p.putString("pcname", cfg.pcName) == cfg.pcName.length();
  ok &= p.putString("apsuffix", cfg.apSuffix) == cfg.apSuffix.length();
  ok &= p.putString("appw", cfg.apPw) == cfg.apPw.length();
  p.end();
  // Kullanıcı kaydettiğini sanır, cihaz yeniden başlayınca eski ayarlarda açılır.
  // The user thinks they've saved their settings, but when the device restarts, it opens with the old settings.

  if (!ok) Serial.println("[cfg] ERROR: Settings could not be written to the NVS");
  return ok;
}

const char *checkDbUrl(const String &in, String &out)
{
  if (in.length() == 0) return "adres bos";
  if (in.length() > 200) return "adres cok uzun";

  String h = in;
  if (h.startsWith("https://")) h = h.substring(8);
  else if (h.startsWith("http://")) return "https olmali";
  while (h.endsWith("/")) h.remove(h.length() - 1);

  if (h.length() == 0) return "adres bos";

  //   @ -> userinfo
  //   / -> path
  //   : -> port redirect
  //   ? # -> query/fragment injection
  for (unsigned i = 0; i < h.length(); i++)
  {
    char c = h[i];
    if (c == '@' || c == '/' || c == ':' || c == '?' || c == '#' ||
        c == '\\' || c <= 0x20 || c >= 0x7F)
      return "adreste gecersiz karakter";
  }

  String low = h;
  low.toLowerCase();

  const char *suffixes[] = { ".firebasedatabase.app", ".firebaseio.com" };
  for (const char *suf : suffixes)
  {
    unsigned n = strlen(suf);
    if (!low.endsWith(suf)) continue;
    if (low.length() <= n) return "eksik alan adi";
    if (low[low.length() - n - 1] == '.') return "eksik alan adi";
    out = h;
    return nullptr;
  }
  return "Firebase adresi degil";
}

const char *checkDeviceName(const String &in)
{
  if (in.length() == 0)  return "cihaz adi bos";
  if (in.length() > 32)  return "cihaz adi cok uzun";
  for (unsigned i = 0; i < in.length(); i++) {
    char c = in[i];
    bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '_' || c == '-';
    if (!ok) return "cihaz adinda sadece harf, rakam, _ ve - olabilir";
  }
  return nullptr;
}

// WPA2 rule
static bool wpaShapeOk(const String &in, int &why)
{
  if (in.length() < 8)  { why = 1; return false; }
  if (in.length() > 32) { why = 2; return false; }
  for (unsigned i = 0; i < in.length(); i++)
    if (in[i] < 0x20 || in[i] > 0x7E) { why = 3; return false; }
  return true;
}

const char *checkApPw(const String &in)
{
  int why = 0;
  if (wpaShapeOk(in, why)) return nullptr;
  if (why == 1) return "hotspot parolasi en az 8 karakter olmali";
  if (why == 2) return "hotspot parolasi en fazla 32 karakter olabilir";
  return "hotspot parolasinda gecersiz karakter";
}

const char *checkPanelPw(const String &in)
{
  int why = 0;
  if (wpaShapeOk(in, why)) return nullptr;
  if (why == 1) return "parola en az 8 karakter olmali";
  if (why == 2) return "parola en fazla 32 karakter olabilir";
  return "parolada gecersiz karakter";
}

const char *checkPcName(const String &in)
{
  if (in.length() > 32) return "bilgisayar adi cok uzun";
  for (unsigned i = 0; i < in.length(); i++)
    if ((unsigned char)in[i] < 0x20) return "bilgisayar adinda gecersiz karakter";
  return nullptr;
}

const char *checkApSuffix(const String &in)
{
  if (in.length() == 0)  return "hotspot adi bos";
  if (in.length() > 16)  return "hotspot adi cok uzun";
  for (unsigned i = 0; i < in.length(); i++)
  {
    char c = in[i];
    bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
    if (!ok) return "hotspot adinda sadece harf, rakam, _ ve - olabilir";
  }
  return nullptr;
}

bool validPath(const String &path)
{
  if (path.length() < 2 || path.length() > 128) return false;
  if (path[0] != '/') return false;
  if (path.endsWith("/")) return false;

  int seg = 0;
  for (unsigned i = 1; i < path.length(); i++)
  {
    char c = path[i];
    if (c == '/') {
      if (seg == 0) return false;
      seg = 0;
      continue;
    }
    // RTDB anahtarlarinda yasak olanlar + URL'i bozanlar
    // Values forbidden in RTDB keys and those that break the URL
    if (c == '.' || c == '$' || c == '#' || c == '[' || c == ']' || c == '?' || c == '&' || c == '%' || c == '@' || c <= 0x20 || c >= 0x7F)
      return false;
    seg++;
  }
  return seg > 0;
}

const char *checkIp(const String &in)
{
  if (in.length() == 0) return nullptr;
  int parts = 0, val = -1;
  for (unsigned i = 0; i <= in.length(); i++)
  {
    if (i == in.length() || in[i] == '.')
    {
      if (val < 0 || val > 255) return "gecersiz IP";
      parts++;
      val = -1;
    }
    else if (in[i] >= '0' && in[i] <= '9')
    {
      val = (val < 0 ? 0 : val * 10) + (in[i] - '0');
      if (val > 255) return "gecersiz IP";
    }
    else
    {
      return "gecersiz IP";
    }
  }
  return parts == 4 ? nullptr : "gecersiz IP";
}

}
