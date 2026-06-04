#pragma once
#include <stdint.h>

#define REG32(addr) (*reinterpret_cast<volatile uint32_t*>(addr))

namespace Drivers {

class Button {
public:
  static void init() {
    REG32(RCC_AHB1ENR) |= (1 << 2);
    REG32(GPIOC_MODER) &= ~(3 << 26);
  }

  static bool is_pressed() {
    return REG32(GPIOC_IDR) & (1 << 13) == 0;
  }

private:
  static constexpr uint32_t RCC_AHB1ENR = 0x40023830;
  static constexpr uint32_t GPIOC_BASE = 0x40020800;

  static constexpr uint32_t GPIOC_MODER = (GPIOC_BASE + 0x00);
  static constexpr uint32_t GPIOC_IDR = (GPIOC_BASE + 0x10);
};

} // namespace Drivers
