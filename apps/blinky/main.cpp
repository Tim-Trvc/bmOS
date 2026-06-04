#include <stdint.h>

#include "button.hpp"
#include "coroutine.hpp"
#include "led.hpp"
#include "task.hpp"

Kernel::Task blinky_coro() {
  Drivers::Led::init();
  while (true) {
    Drivers::Led::toggle();
    co_await std::suspend_always{};
  }
}

Kernel::Task button_coro() {
  Drivers::Button::init();
  while (true) {
    co_await std::suspend_always{};
  }
}

volatile uint32_t system_tick = 0;

extern "C" void SysTick_Handler(void) { system_tick = system_tick + 1; }

int main() {
  Kernel::Task blinky_task = blinky_coro();
  Kernel::Task button_task = button_coro();

  while (true) {
    blinky_task.handle.resume();
    button_task.handle.resume();
  }
}
