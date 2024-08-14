#pragma once
#include <cotask/basic_awaitable.hpp>
#include <cotask/this_thread.hpp>

namespace cotask {

struct generate_task_awaitable : cotask::basic_awaitable {
  cotask::operation_context op;
  bool completed{false};

  generate_task_awaitable() = delete;
  generate_task_awaitable(const generate_task_awaitable&) = delete;
  generate_task_awaitable(generate_task_awaitable&&) = default;

  template <typename Task_functor>
  generate_task_awaitable(Task_functor f)
      : op{[&]() {
          auto result = cotask::make_operation_context(
              [f = std::move(f), this]() -> cotask::task<void> {
                f();
                completed = true;
                co_return;
              },
              std::chrono::milliseconds{1500}, true, true);
          cotask::threading::this_thread::cc.attach(op);
          return result;
        }()} {}

  generate_task_awaitable& operator=(const generate_task_awaitable&) = delete;
  generate_task_awaitable& operator=(generate_task_awaitable&&) = default;

  bool await_ready() { return completed; }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() { return completed; });
  }

  void await_resume() { reset_handle(); }
};

}  // namespace cotask