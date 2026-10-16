// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

/*
  JSON string quote ve unquote. Hem panel hem rtdb kullanıyor.
  Veriler hatasız yazılıp okunabilmesi için ortak yapıda.

  JSON string quotes and unquotes. It uses both the panel and the RTDB.
  The data follows a common structure to ensure it can be written and read without errors.
*/

namespace json
{

inline String quote(const String &s)
{
  String o = "\"";
  char hex[7];
  for (unsigned i = 0; i < s.length(); i++)
  {
    uint8_t c = (uint8_t)s[i];
    switch (c) {
      case '"':  o += "\\\""; break;
      case '\\': o += "\\\\"; break;
      case '\n': o += "\\n";  break;
      case '\r': o += "\\r";  break;
      case '\t': o += "\\t";  break;
      case '\b': o += "\\b";  break;
      case '\f': o += "\\f";  break;
      default:
        if (c < 0x20) { snprintf(hex, sizeof(hex), "\\u%04x", c); o += hex; }
        else { o += (char)c; }
    }
  }
  return o + "\"";
}

inline uint8_t hexVal(char c)
{
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return 0xFF;
}

inline String unquote(const String &s)
{
  String o;
  o.reserve(s.length());
  for (unsigned i = 0; i < s.length(); i++)
  {
    if (s[i] != '\\' || i + 1 >= s.length()) { o += s[i]; continue; }
    char e = s[++i];
    switch (e)
    {
      case 'n': o += '\n'; break;
      case 'r': o += '\r'; break;
      case 't': o += '\t'; break;
      case 'b': o += '\b'; break;
      case 'f': o += '\f'; break;
      case 'u': 
      {
        if (i + 4 >= s.length()) return o;
        uint16_t cp = 0;
        bool ok = true;
        for (int k = 1; k <= 4; k++)
        {
          uint8_t v = hexVal(s[i + k]);
          if (v == 0xFF) { ok = false; break; }
          cp = (cp << 4) | v;
        }
        if (!ok) { o += "\\u"; break; }
        i += 4;
        if (cp < 0x80) { o += (char)cp; }
        else if (cp < 0x800)
        {
          o += (char)(0xC0 | (cp >> 6));
          o += (char)(0x80 | (cp & 0x3F));
        }
        else
        {
          o += (char)(0xE0 | (cp >> 12));
          o += (char)(0x80 | ((cp >> 6) & 0x3F));
          o += (char)(0x80 | (cp & 0x3F));
        }
        break;
      }
      default: o += e;
    }
  }
  return o;
}

}
