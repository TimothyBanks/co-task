#define BOOST_TEST_MODULE select_context_test

#include <atomic>
#include <boost/test/included/unit_test.hpp>
#include <cotask/any_task.hpp>
#include <cotask/awaitable.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>
#include <cotask/task.hpp>
#include <mutex>
#include <thread>

struct acquire_lock_awaitable : cotask::basic_awaitable {
  std::unique_lock<std::mutex> guard;
  std::coroutine_handle<> parent;

  acquire_lock_awaitable() = delete;
  acquire_lock_awaitable(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable(acquire_lock_awaitable&&) = default;
  acquire_lock_awaitable(std::unique_lock<std::mutex> guard_)
      : guard{std::move(guard_)} {}

  acquire_lock_awaitable& operator=(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable& operator=(acquire_lock_awaitable&&) = default;

  bool await_ready() { return guard.try_lock(); }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_ready_functor(h, [&]() { return guard.try_lock(); });
    parent = h;
  }

  void await_resume() { reset_handle(); }
};

cotask::task<void> foobar() {
  std::cout << "foobar is invoked from thread " << std::this_thread::get_id()
            << std::endl;
  co_await std::suspend_always{};
  std::cout << "foobar is is back from suspension from thread "
            << std::this_thread::get_id() << std::endl;
  co_return;
}

std::mutex write_mutex;

std::function<cotask::any_task(void)> make_task(
    std::string id,
    std::chrono::milliseconds timeout) {
  return [id = std::move(id), timeout]() -> cotask::task<void> {
    auto lock_awaitable = acquire_lock_awaitable{
        std::unique_lock<std::mutex>{write_mutex, std::defer_lock}};
    co_await lock_awaitable;
    std::this_thread::sleep_for(timeout);
    std::cout << "hello from me " << id << " thread id "
              << std::this_thread::get_id() << std::endl;
    auto fb = foobar();
    co_await cotask::awaitable{fb};
    std::cout << "hello from me after suspension " << id << " thread id "
              << std::this_thread::get_id() << std::endl;
    co_await std::suspend_always{};
    std::cout << "hello from me after another suspension " << id
              << " thread id " << std::this_thread::get_id() << std::endl;
    co_return;
  };
}

BOOST_AUTO_TEST_CASE(cotask_tests) {
  auto i = size_t{0};

  auto ocs = std::vector<cotask::operation_context>{
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
  };

  auto cc = cotask::make_cotask_context();
  for (auto& oc : ocs) {
    cc.attach(oc);
  }
  cc.start();
}