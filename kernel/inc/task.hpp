#pragma once

#include <stddef.h>
#include <stdint.h>

#include "coroutine.hpp"
#include "led.hpp"

namespace Kernel {

class Task {
 public:
  struct promise_type {
    uint32_t wake_up_tick = 0;

    static void* operator new(size_t size) {
      alignas(8) static uint8_t coroutine_pool[2048];
      static size_t offset = 0;

      offset = (offset + 7) & ~7;

      if (offset + size > sizeof(coroutine_pool)) {
        Drivers::Led::init();
        while (true) {
        }
      }

      void* ptr = &coroutine_pool[offset];
      offset += size;
      return ptr;
    }

    static void operator delete(void* ptr, size_t size) {}

    Task get_return_object() {
      return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {};
    void unhandled_exception() {
      while (true) {
      }
    }
  };

  std::coroutine_handle<promise_type> handle;

  Task(std::coroutine_handle<promise_type> h) : handle(h) {}

  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  Task(Task&& other) noexcept : handle(other.handle) { other.handle = {}; }

  Task& operator=(Task&& other) noexcept {
    if (this != &other) {
      if (handle) handle.destroy();
      handle = other.handle;
      other.handle = {};
    }
    return *this;
  }

  ~Task() {
    if (handle) {
      handle.destroy();
    }
  }
};

struct sleep_for {
  uint32_t duration_ms;
  sleep_for(uint32_t ms) : duration_ms(ms) {}

  bool await_ready() const noexcept { return false; }

  template <typename PromiseType>
  void await_suspend(std::coroutine_handle<PromiseType> handle) const noexcept {
    extern volatile uint32_t system_tick;
    handle.promise().wake_up_tick = system_tick + duration_ms;
  }

  void await_resume() const noexcept {}
};

}  // namespace Kernel
