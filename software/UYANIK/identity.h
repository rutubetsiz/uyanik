// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

/*
  Cihaza özgü credentials, ilk açılışta tamamen rastgele oluşturuluyor.

  Credentials are generated completely at random during the first startup.
*/

namespace identity
{

void begin(uint8_t resetButtonPin);

const String &apSsid();
const String &panelCredential();
const String &otaCredential();

void printLabel(); // Her açılışta serial porta basar. It writes to the serial port on every startup.

void factoryReset();

void checkResetButton(); // Buton 5 sn basılı tutulursa factoryReset() çalışır. If the button is held for 5 seconds, factoryReset() runs.

}
