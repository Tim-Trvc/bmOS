#include <stdint.h>

#include "button.hpp"
#include "coroutine.hpp"
#include "led.hpp"
#include "systick.hpp"
#include "task.hpp"

Kernel::Task blinky_coro() {
  while (true) {
    Drivers::Led::toggle();
    co_await Kernel::sleep_for(500);
  }
}

Kernel::Task button_coro() {
  while (true) {
    co_await Kernel::sleep_for(500);
  }
}

namespace Kernel {
volatile uint32_t system_tick = 0;
}  // namespace Kernel

extern "C" void SysTick_Handler(void) {
  Kernel::system_tick = Kernel::system_tick + 1;
}
extern "C" void HardFault_Handler(void) {
  while (true) {
  }
}

int main() {
  Drivers::SysTick::init();
  Drivers::Led::init();
  Drivers::Button::init();

  Kernel::Task blinky_task = blinky_coro();
  Kernel::Task button_task = button_coro();

  if (!blinky_task.handle || !button_task.handle) {
    while (true) {
      Drivers::Led::toggle();
      for (volatile int i = 0; i < 100000; i++);
    }
  }

  while (true) {
    if (blinky_task.handle && !blinky_task.handle.done()) {
      if (Kernel::system_tick > blinky_task.handle.promise().wake_up_tick) {
        blinky_task.handle.resume();
      }
    }

    if (button_task.handle && !button_task.handle.done()) {
      if (Kernel::system_tick > button_task.handle.promise().wake_up_tick) {
        button_task.handle.resume();
      }
    }
  }
}
