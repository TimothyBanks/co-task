#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/cotask_exception.hpp>
#include <tbb/tbb.h>

namespace cotask {

struct cotask_context_impl {
    // std::atomic<context_status> status;
    // boost::asio::io_context io;
    // boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
    // tbb::task_arena arena;
    // tbb::task_group group;

    void start() {}
    void stop() {}
};

cotask_context make_cotask_context() {
    auto context = cotask_context{};
    // context.impl = std::make_shared<cotask_context_impl>();
    return context;
}

cotask_context::cotask_context() noexcept = default;
cotask_context::cotask_context(const cotask_context&) noexcept = default;
cotask_context::cotask_context(cotask_context&&) noexcept = default;

cotask_context& cotask_context::operator=(const cotask_context&) noexcept = default;
cotask_context& cotask_context::operator=(cotask_context&&) noexcept = default;

void cotask_context::start() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->start();
}

void cotask_context::stop() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->stop();
}

// std::atomic<context_status>& cotask_context::status() {
//     if (!impl) {
//         throw cotask_exception{"impl is invalid"};
//     }
//     return impl->status;
// }

// const std::atomic<context_status>& cotask_context::status() const {
//     if (!impl) {
//         throw cotask_exception{"impl is invalid"};
//     }
//     return impl->status;
// }

}