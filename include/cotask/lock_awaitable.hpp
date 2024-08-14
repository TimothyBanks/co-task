#pragma once
#include <cotask/basic_awaitable.hpp>

namespace cotask {

struct lock_awaitable : cotask::basic_awaitable {
  std::unique_lock<std::mutex> guard;

  lock_awaitable() = delete;
  lock_awaitable(const lock_awaitable&) = delete;
  lock_awaitable(lock_awaitable&&) = default;
  lock_awaitable(std::unique_lock<std::mutex> guard_)
      : guard{std::move(guard_)} {}

  lock_awaitable& operator=(const lock_awaitable&) = delete;
  lock_awaitable& operator=(lock_awaitable&&) = default;

  bool await_ready() { return guard.try_lock(); }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() { return guard.try_lock(); });
  }

  void await_resume() { reset_handle(); }
};

}  // namespace cotask