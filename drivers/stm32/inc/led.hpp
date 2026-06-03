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

  static void toggle() {
    REG32(GPIOA_ODR) ^= (1 << 5);
  }

  static void delay(int iterations) {
    for (volatile int i=0; i<iterations; i = i + 1) {}
  }

private:
  static constexpr uint32_t RCC_BASE = 0x40023800;
  static constexpr uint32_t GPIOA_BASE = 0x40020000;

  static constexpr uint32_t RCC_AHB1ENR = (RCC_BASE + 0x30);
  static constexpr uint32_t GPIOA_MODER = (GPIOA_BASE + 0x00);
  static constexpr uint32_t GPIOA_ODR = (GPIOA_BASE + 0x14);
};

} // namespace Drivers
