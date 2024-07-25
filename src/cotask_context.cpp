#include <cotask/cotask_context.hpp>
#include <cotask/cotask_exception.hpp>

namespace cotask {

struct cotask_context_impl {
    std::atomic<context_status> status = context_status::stopped;
    boost::asio::io_context io;
    tbb::task_arena arena;
    tbb::task_group group;

    void start() {}
    void stop() {}
};

cotask_context make_cotask_context() {
    auto context = cotask_context{};
    context.impl = std::make_shared<cotask_context_impl>();
    return context;
}

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

std::atomic<context_status>& cotask_context::status() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->status;
}

const std::atomic<context_status>& cotask_context::status() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->status;
}

boost::asio::io_context& cotask_context::io() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->io;
}

const boost::asio::io_context& cotask_context::io() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->io;
}

tbb::task_arena& cotask_context::arena() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->arena;
}

const tbb::task_arena& cotask_context::arena() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->arena;
}

tbb::task_group& cotask_context::group() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->group;
}

const tbb::task_group& cotask_context::group() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->group;
}

}