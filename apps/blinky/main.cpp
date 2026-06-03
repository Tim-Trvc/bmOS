#include <stdint.h>

#include "led.hpp"

int main() {
  Drivers::Led::init();

  while (true) {
    Drivers::Led::toggle(); 
    
    Drivers::Led::delay(50000);
  }
}
