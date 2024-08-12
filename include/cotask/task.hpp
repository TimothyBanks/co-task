#pragma once
#include <coroutine>
#include <functional>
#include <optional>

namespace cotask {

template <typename T, typename Enabled = void>
struct basic_promise_type;

template <typename T>
struct basic_promise_type<T, std::enable_if_t<std::is_void_v<T>>> {
  std::exception_ptr exception;
  void return_void() {}
};

template <typename T>
struct basic_promise_type<T, std::enable_if_t<!std::is_void_v<T>>> {
  std::exception_ptr exception;
  std::optional<T> value;

  T result() { return value.value_or(T{}); }

  void return_value(T v) { value = std::move(v); }
};

template <typename T>
struct task {
  struct promise_type : basic_promise_type<T> {
    std::function<bool(void)> ready = []() { return true; };

    std::suspend_always initial_suspend() { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    void unhandled_exception() { this->exception = std::current_exception(); }

    task<T> get_return_object() {
      return task<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
    }
  };

  std::coroutine_handle<promise_type> handle{nullptr};

  task() = default;
  task(const task&) = delete;
  task(task&& other) : handle{std::exchange(other.handle, {})} {}
  task(std::coroutine_handle<promise_type> h) : handle{h} {}

  ~task() {
    if (handle) {
      handle.destroy();
    }
  }

  task& operator=(const task&) = delete;
  task& operator=(task&& other) {
    if (this != &other) {
      handle = std::exchange(other.handle, {});
    }
    return *this;
  };

  void resume() {
    if (handle.done()) {
      return;
    }
    if (!handle.promise().ready()) {
      return;
    }
    handle.resume();
  }

  bool done() const { return handle.done(); }
};

}  // namespace cotask