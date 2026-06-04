// Copyright 2026 Tim Travica
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

#pragma once
#include <stdint.h>

#define REG32(addr) (*reinterpret_cast<volatile uint32_t*>(addr))

namespace Drivers {

class Led {
 public:
  static void init() {
    REG32(RCC_AHB1ENR) |= (1 << 0);
    REG32(GPIOA_MODER) &= ~(1 << 11);
    REG32(GPIOA_MODER) |= (1 << 10);
  }

  static void toggle() { REG32(GPIOA_ODR) ^= (1 << 5); }

  static void delay(int iterations) {
    for (volatile int i = 0; i < iterations; i = i + 1) {
    }
  }

 private:
  static constexpr uint32_t RCC_BASE = 0x40023800;
  static constexpr uint32_t GPIOA_BASE = 0x40020000;

  static constexpr uint32_t RCC_AHB1ENR = (RCC_BASE + 0x30);
  static constexpr uint32_t GPIOA_MODER = (GPIOA_BASE + 0x00);
  static constexpr uint32_t GPIOA_ODR = (GPIOA_BASE + 0x14);
};

}  // namespace Drivers
