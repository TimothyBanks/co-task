#pragma once
#include <cotask/basic_awaitable.hpp>

namespace cotask {

struct task_awaitable : basic_awaitable {
  std::coroutine_handle<> handle;

  task_awaitable() = default;
  task_awaitable(const task_awaitable&) = default;
  task_awaitable(task_awaitable&&) = default;

  template <typename T>
  task_awaitable(const task<T>& t) : handle{t.handle} {}

  task_awaitable& operator=(const task_awaitable&) = default;
  task_awaitable& operator=(task_awaitable&&) = default;

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