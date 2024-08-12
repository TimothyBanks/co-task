#pragma once
#include <coroutine>
#include <cotask/task.hpp>

namespace cotask {

struct awaitable {
  std::coroutine_handle<> handle;
  std::coroutine_handle<> parent;
  std::function<void(void)> reset_promise;

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
    h.promise().ready = [&]() {
      if (handle && !handle.done()) {
        handle.resume();
      }
      return handle.done();
    };
    reset_promise = [h]() { h.promise().ready = []() { return true; }; };
    parent = h;
  }

  void await_resume() noexcept { reset_promise(); }
};

}  // namespace cotask