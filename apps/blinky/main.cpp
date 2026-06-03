#include <stdint.h>

constexpr uint32_t RCC_BASE = 0x40023800;
constexpr uint32_t GPIOA_BASE = 0x40020000;

volatile uint32_t& rcc_ahb1enr = *reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x30);
volatile uint32_t& gpioa_moder = *reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x00);
volatile uint32_t& gpioa_odr   = *reinterpret_cast<volatile uint32_t*>(GPIOA_BASE + 0x14);

int main() {
  
  rcc_ahb1enr |= (1 << 0);
  
  gpioa_moder &= ~(1 << 11);
  gpioa_moder |= (1 << 10);

  while (true) {
    gpioa_odr ^= (1 << 5);
    
    for (volatile int i = 0; i < 500000; ++i) {
    }
  }
}
