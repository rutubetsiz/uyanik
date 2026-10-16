// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

// Güç ve Reset düğmesine basma.
// Pressing the Power and Reset buttons.

namespace press
{

void begin(uint8_t powerPin, uint8_t resetPin);

// Basımı başlatır ve hemen döner. Zaten bir basım sürüyorsa false. 
// Starts a pressing and returns immediately. Returns false if a press is already in progress.

bool power(uint32_t ms);
bool reset(uint32_t ms);

bool busy();

// Son basımdan bu yana bırakma oldu mu? Bir kez true döner.
// Has there been a dropout since the last press? Returns “true” once.

bool takeRelease();

// Basım bittiğinde cağrılır.
// Called when press is complete.
void onRelease(void (*cb)());

}
