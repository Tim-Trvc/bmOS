#pragma once
#include <stdint.h>

#define REG32(addr) (*reinterpret_cast<volatile uint32_t*>(addr))

namespace Drivers {

class SysTick {
public:
  static void init() {
    REG32(SYSTICK_RVR) = 16000 - 1;
    REG32(SYSTICK_CVR) &= 0;
    REG32(SYSTICK_CSR) |= (1 << 0) | (1 << 1) | (1 << 2);

  }

private:
  static constexpr uint32_t SYSTICK_BASE = 0xE000E010;

  static constexpr uint32_t SYSTICK_CSR = (SYSTICK_BASE + 0x00);
  static constexpr uint32_t SYSTICK_RVR = (SYSTICK_BASE + 0x04);
  static constexpr uint32_t SYSTICK_CVR = (SYSTICK_BASE + 0x08);
};

} // namespace Drivers
