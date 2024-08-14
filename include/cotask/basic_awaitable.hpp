#pragma once
#include <coroutine>
#include <cotask/task.hpp>

namespace cotask {

struct basic_awaitable {
  using ready_functor = std::function<bool(void)>;
  using reset_functor = std::function<void(void)>;

  reset_functor reset = []() {};
  std::coroutine_handle<> awaiting_handle;

  template <typename T>
  void set_awaiting_handle(std::coroutine_handle<T> handle,
                           ready_functor functor) {
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

}  // namespace cotask