#define BOOST_TEST_MODULE select_context_test

#include <atomic>
#include <boost/test/included/unit_test.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>
#include <thread>

BOOST_AUTO_TEST_CASE(cotask_tests) {
    auto i = std::atomic<uint64_t>{0};

    auto body = [&](){
        auto me = i.fetch_add(1);
        std::this_thread::sleep_for(std::chrono::seconds{2});
        std::cout << "hello from me " << me << " thread id " << std::this_thread::get_id() << std::endl;
    };

    auto ocs = std::vector<cotask::operation_context>{
        cotask::make_operation_context(body, std::chrono::milliseconds{0}, false, false),
        cotask::make_operation_context(body, std::chrono::milliseconds{0}, false, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
        // cotask::make_operation_context(body, std::chrono::milliseconds{1000}, true, false),
    };

    auto cc = cotask::make_cotask_context();
    for (auto& oc : ocs) {
        cc.attach(oc);
    }
    cc.start();
}