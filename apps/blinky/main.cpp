#include <stdint.h>

#include "led.hpp"
#include "systick.hpp"

class BlinkyTask {
 public:
  void run(uint32_t current_tick) {
    if (!is_initialized) {
      Drivers::Led::init();
      is_initialized = true;
    }

    if (current_tick - last_action_tick >= wait_ticks) {
      Drivers::Led::toggle();
      last_action_tick = current_tick;
    }
  }

 private:
  bool is_initialized = false;
  uint32_t last_action_tick = 0;
  uint32_t wait_ticks = 500;
};

volatile uint32_t system_tick = 0;

extern "C" void SysTick_Handler(void) { system_tick += 1; }

int main() {
  BlinkyTask blinky;

  Drivers::SysTick::init();

  while (true) {
    blinky.run(system_tick);
  }
}
