#pragma once

#include <stddef.h>
#include <stdint.h>

#include "coroutine.hpp"

namespace Kernel {

class Task {
 public:
  struct promise_type {
    static void* operator new(size_t size) {
      static uint8_t coroutine_pool[2048];
      static size_t offset = 0;

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
    void undhandled_exception() {
      while (true) {
      }
    }
  };

  std::coroutine_handle<promise_type> handle;
  Task(std::coroutine_handle<promise_type> h) : handle(h) {}
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
