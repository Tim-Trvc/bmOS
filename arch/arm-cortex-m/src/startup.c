#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sdata, _edata, _sidata;
extern uint32_t _sbss, _ebss;

extern int main(void);
extern void SysTick_Handler(void);
extern void HardFault_Handler(void);

void Default_Handler(void) {
  while (1) {
  }
}

void Reset_Handler(void) {
  uint32_t *src = &_sidata;
  uint32_t *dst = &_sdata;
  while (dst < &_edata) {
    *dst++ = *src++;
  }

  dst = &_sbss;
  while (dst < &_ebss) {
    *dst++ = 0;
  }

  main();
  while (1);
}

__attribute__((section(".isr_vector"))) void (*const vector_table[])(void) = {
    (void (*)(void))(&_estack),  // start adress of stack
    Reset_Handler,               // Reset (on startup)
    Default_Handler,             // NMI
    HardFault_Handler,           // HardFault
    Default_Handler,             // MemManage
    Default_Handler,             // BusFault
    Default_Handler,             // UsageFault
    0,
    0,
    0,
    0,                // ARM reserved
    Default_Handler,  // SVCall
    Default_Handler,  // Debug Monitor
    0,                // reserved
    Default_Handler,  // PendSV
    SysTick_Handler   // SysTick
};
