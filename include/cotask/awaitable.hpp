#pragma once
#include <coroutine>
#include <cotask/task.hpp>

namespace cotask {

struct basic_awaitable {
  using ready_functor = std::function<bool(void)>;
  using reset_functor = std::function<void(void)>;

  reset_functor reset;

  template <typename T>
  void set_ready_functor(std::coroutine_handle<T> handle, ready_functor functor) {
    handle.promise().ready = std::move(functor);
    reset = [handle]() { handle.promise().ready = []() { return true; }; };
  }

  void reset_handle() {
    reset();
  }
};

struct awaitable : basic_awaitable {
  std::coroutine_handle<> handle;
  std::coroutine_handle<> parent;

  awaitable() = default;
  awaitable(const awaitable&) = default;
  awaitable(awaitable&&) = default;

  template <typename T>
  awaitable(const task<T>& t) : handle{t.handle} {}

  awaitable& operator=(const awaitable&) = default;
  awaitable& operator=(awaitable&&) = default;

  bool await_ready() const {
    if (!handle) {
      return true;
    }
    if (!handle.done()) {
      handle.resume();
    }
    return handle.done();
  }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_ready_functor(h, [&]() {
      if (handle && !handle.done()) {
        handle.resume();
      }
      return handle.done();
    });
    parent = h;
  }

  void await_resume() noexcept { reset_handle(); }
};

}  // namespace cotask