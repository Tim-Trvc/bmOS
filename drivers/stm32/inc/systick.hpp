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

class SysTick {
 public:
  static void init() {
    REG32(SYSTICK_RVR) = 16000 - 1;
    REG32(SYSTICK_CVR) = 0;
    REG32(SYSTICK_CSR) |= (1 << 0) | (1 << 1) | (1 << 2);
  }

 private:
  static constexpr uint32_t SYSTICK_BASE = 0xE000E010;

  static constexpr uint32_t SYSTICK_CSR = (SYSTICK_BASE + 0x00);
  static constexpr uint32_t SYSTICK_RVR = (SYSTICK_BASE + 0x04);
  static constexpr uint32_t SYSTICK_CVR = (SYSTICK_BASE + 0x08);
};

}  // namespace Drivers
