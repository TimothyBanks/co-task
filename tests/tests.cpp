#define BOOST_TEST_MODULE select_context_test

#include <atomic>
#include <boost/test/included/unit_test.hpp>
#include <cotask/any_task.hpp>
#include <cotask/async_awaitable.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/task_awaitable.hpp>
#include <cotask/lock_awaitable.hpp>
#include <cotask/operation_context.hpp>
#include <cotask/task.hpp>
#include <mutex>
#include <thread>

cotask::cotask_context cc = cotask::make_cotask_context();
std::unique_ptr<std::mutex> write_mutex = std::make_unique<std::mutex>();

cotask::task<void> foobar(const std::string& id) {
  std::cout << "[" << id << ", " << std::this_thread::get_id()
            << "] foobar invoked." << std::endl;
  co_await std::suspend_always{};
  std::cout << "[" << id << ", " << std::this_thread::get_id()
            << "] foobar after suspension." << std::endl;
  co_return;
}

std::function<cotask::any_task(void)> make_task(
    std::string id,
    std::chrono::milliseconds timeout) {
  return [id = std::move(id), timeout]() -> cotask::task<void> {
    auto lock_awaitable = cotask::lock_awaitable{
        std::unique_lock<std::mutex>{*write_mutex, std::defer_lock}};
    co_await lock_awaitable;
    std::cout << "[" << id << ", " << std::this_thread::get_id()
              << "] Lock acquired. Going to sleep." << std::endl;

    auto tid = std::this_thread::get_id();
    auto async_await = cotask::async_awaitable{[&] {
      std::cout << "[" << id << ", " << std::this_thread::get_id()
                << "] Ran asynchronous operation. " << std::endl;
    }};
    co_await async_await;

    auto fb = foobar(id);
    co_await cotask::task_awaitable{fb};
    std::cout << "[" << id << ", " << std::this_thread::get_id()
              << "] Coming back after first suspension " << std::endl;
    co_await std::suspend_always{};
    std::cout << "[" << id << ", " << std::this_thread::get_id()
              << "] Coming back after last suspension " << std::endl;
    co_return;
  };
}

BOOST_AUTO_TEST_CASE(cotask_tests) {
  auto i = size_t{0};

  auto ocs = std::vector<cotask::operation_context>{
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{10000}),
          std::chrono::milliseconds{1500}, false, false),
  };

  for (auto& oc : ocs) {
    cc.attach(oc);
  }
  cc.start();
}