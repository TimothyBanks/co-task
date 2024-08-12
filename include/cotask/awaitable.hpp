#pragma once
#include <coroutine>

namespace cotask {

struct awaitable {
  std::coroutine_handle<> handle;
  bool is_suspended = false;

  bool await_ready() const noexcept { return !is_suspended; }

  void await_suspend(std::coroutine_handle<> h) noexcept {
    handle = h;
    is_suspended = true;
  }

  void await_resume() const noexcept {}

  void resume() {
    if (is_suspended && handle) {
      handle.resume();
      is_suspended = false;
    }
  }
};

}  // namespace cotask