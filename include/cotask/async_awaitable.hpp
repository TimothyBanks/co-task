#pragma once
#include <cotask/basic_awaitable.hpp>
#include <future>

namespace cotask {

struct async_awaitable : cotask::basic_awaitable {
  bool completed{false};

  async_awaitable() = delete;
  async_awaitable(const async_awaitable&) = delete;
  async_awaitable(async_awaitable&&) = default;

  template <typename Thread_functor>
  async_awaitable(Thread_functor f) {
    auto h = std::async(std::launch::async, [f = std::move(f), this]() {
      f();
      completed = true;
    });
  }

  async_awaitable& operator=(const async_awaitable&) = delete;
  async_awaitable& operator=(async_awaitable&&) = default;

  bool await_ready() { return completed; }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() { return completed; });
  }

  void await_resume() { reset_handle(); }
};

}  // namespace cotask