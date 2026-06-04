#pragma once

namespace std {

template <typename Promise = void>
struct coroutine_handle;

template <>
struct coroutine_handle<void> {
  void* ptr{nullptr};

  void resume() { __builtin_coro_resume(ptr); }
  void done() const { __builtin_coro_done(ptr); }
};

template <typename Promise>
struct coroutine_handle : coroutine_handle<void> {
  static coroutine_handle from_promise(Promise& p) {
    coroutine_handle h;
    h.ptr = __builtin_coro_promise(&p, alignof(Promise), false);
    return h;
  }
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
