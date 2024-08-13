#pragma once
#include <coroutine>
#include <cotask/task.hpp>

namespace cotask {

struct basic_awaitable {
  using ready_functor = std::function<bool(void)>;
  using reset_functor = std::function<void(void)>;

  reset_functor reset = [](){};
  std::coroutine_handle<> awaiting_handle;

  template <typename T>
  void set_awaiting_handle(std::coroutine_handle<T> handle, ready_functor functor) {
    handle.promise().ready = std::move(functor);
    reset = [handle]() { handle.promise().ready = []() { return true; }; };
    awaiting_handle = handle;
  }

  void reset_handle() {
    if (!reset) {
      return;
    }
    reset();
  }
};

struct awaitable : basic_awaitable {
  std::coroutine_handle<> handle;

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
    set_awaiting_handle(h, [&]() {
      if (handle && !handle.done()) {
        handle.resume();
      }
      return handle.done();
    });
  }

  void await_resume() noexcept { reset_handle(); }
};

}  // namespace cotask