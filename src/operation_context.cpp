#include <cotask/cotask_exception.hpp>
#include <cotask/operation_context.hpp>

namespace cotask {

struct operation_context_impl {
    std::function<void(void)> body;
    std::chrono::milliseconds interval;
    bool one_and_done{false};
    bool run_immediately{false};
    boost::asio::steady_timer timer;
};

operation_context make_operation_context() {
    auto context = operation_context{};
    context.impl = std::make_shared<operation_context_impl>();
    return context;
}

operation_context::functor& operation_context::body() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->body;
}

const operation_context::functor& operation_context::body() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->body;
}

std::chrono::milliseconds& operation_context::interval() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->interval;
}

const std::chrono::milliseconds& operation_context::interval() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->interval;
}

bool& operation_context::one_and_done() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->one_and_done;
}

const bool& operation_context::one_and_done() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->one_and_done;
}

bool& operation_context::run_immediately() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->run_immediately;
}

const bool& operation_context::run_immediately() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->run_immediately;
}

boost::asio::steady_timer& operation_context::timer() {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->timer;
}

const boost::asio::steady_timer& operation_context::timer() const {
    if (!impl) {
        throw cotask_exception{"impl is invalid"};
    }
    return impl->timer;
}

}
