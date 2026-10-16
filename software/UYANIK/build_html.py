# Copyright (C) 2026 rutubetsiz
# SPDX-License-Identifier: GPL-3.0-or-later

"""
panel.html -> panel_html.h

python build_html.py            (generate panel_html.h from panel.html)
python build_html.py --kontrol  (check that panel_html.h is up to date)
"""
import sys, os

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, "panel.html")
DST = os.path.join(HERE, "panel_html.h")
DELIM = "HTMLPAGE"

BASLIK = """// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

/*
  Bu dosya üretiliyor. Elle düzenleme yapmayın.
  Kaynak: panel.html
  Üretmek için: python build_html.py
  Kontrol etmek için: python build_html.py --kontrol

  This file is generated. Do not edit it manually.
  Source: panel.html
  To generate: python build_html.py
  To check: python build_html.py --kontrol
*/

const char PANEL_HTML[] PROGMEM = R"%s(""" % DELIM

ALTLIK = ')%s";\n' % DELIM


def uret(html):
    if ")%s" % DELIM in html:
        sys.exit("HATA: panel.html icinde ')%s' geciyor, ham dizge bolunur." % DELIM)
    html = html.replace("\r\n", "\n").rstrip("\n")
    return BASLIK + html + ALTLIK


def main():
    if not os.path.exists(SRC):
        sys.exit("HATA: panel.html yok")
    html = open(SRC, encoding="utf-8").read()
    yeni = uret(html)

    kontrol = "--kontrol" in sys.argv
    eski = open(DST, encoding="utf-8").read() if os.path.exists(DST) else ""

    if kontrol:
        if eski == yeni:
            print("panel_html.h up to date")
            return
        sys.exit("ERROR: panel_html.h is out of date. Run 'python build_html.py'.")

    if eski == yeni:
        print("No changes (%d byte)" % len(yeni))
        return
    open(DST, "w", encoding="utf-8", newline="\n").write(yeni)
    print("generated: panel_html.h  %d byte HTML  ->  %d byte" % (len(html), len(yeni)))


if __name__ == "__main__":
    main()
