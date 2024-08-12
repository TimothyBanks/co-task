#define BOOST_TEST_MODULE select_context_test

#include <atomic>
#include <boost/test/included/unit_test.hpp>
#include <cotask/any_task.hpp>
#include <cotask/awaitable.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>
#include <cotask/task.hpp>
#include <thread>

std::function<cotask::any_task(void)> make_task(std::string id, std::chrono::milliseconds timeout) {
    return [id = std::move(id), timeout]() -> cotask::task<void> {
        std::this_thread::sleep_for(timeout);
        std::cout << "hello from me " << id << " thread id " << std::this_thread::get_id() << std::endl;
        co_await std::suspend_always{};
        std::cout << "hello from me after suspension " << id << " thread id " << std::this_thread::get_id() << std::endl;
        co_await std::suspend_always{};
        std::cout << "hello from me after another suspension " << id << " thread id " << std::this_thread::get_id() << std::endl;
        co_return;
    };
}

BOOST_AUTO_TEST_CASE(cotask_tests) {
    auto i = size_t{0};

    auto ocs = std::vector<cotask::operation_context>{
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
        cotask::make_operation_context(make_task(std::to_string(++i), std::chrono::milliseconds{1}), std::chrono::milliseconds{1}, false, false),
    };

    auto cc = cotask::make_cotask_context();
    for (auto& oc : ocs) {
        cc.attach(oc);
    }
    cc.start();
}