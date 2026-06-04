#pragma once

namespace std {

template <typename Promise = void>
struct coroutine_handle;

template <>
struct coroutine_handle<void> {
  void* ptr{nullptr};

  void resume() { __builtin_coro_resume(ptr); }
  bool done() const { return __builtin_coro_done(ptr); }

  void destroy() { __builtin_coro_destroy(ptr); }

  explicit operator bool() const { return ptr != nullptr; }

  void* address() const { return ptr; }
  static coroutine_handle from_address(void* addr) {
    coroutine_handle h;
    h.ptr = addr;
    return h;
  }
};

template <typename Promise>
struct coroutine_handle : coroutine_handle<void> {
  static coroutine_handle from_promise(Promise& p) {
    coroutine_handle h;
    h.ptr = __builtin_coro_promise(&p, alignof(Promise), true);
    return h;
  }

  static coroutine_handle from_address(void* addr) {
    coroutine_handle h;
    h.ptr = addr;
    return h;
  }

  Promise& promise() const {
    void* promise_ptr =
        __builtin_coro_promise(this->ptr, alignof(Promise), false);
    return *static_cast<Promise*>(promise_ptr);
  }
};

template <typename R, typename... Args>
struct coroutine_traits {
  using promise_type = typename R::promise_type;
};

struct suspend_always {
  bool await_ready() const noexcept { return false; }
  void await_suspend(coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};

struct suspend_never {
  bool await_ready() const noexcept { return true; }
  void await_suspend(coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};

}  // namespace std
