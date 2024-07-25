#pragma once
#include <boost/asio.hpp>
#include <chrono>
#include <functional>
#include <memory>

namespace cotask {

struct operation_context_impl;

struct operation_context {
    using functor = std::function<void(void)>;

    operation_context() noexcept;
    operation_context(const operation_context&) noexcept;
    operation_context(operation_context&&) noexcept;

    operation_context& operator=(const operation_context&) noexcept;
    operation_context& operator=(operation_context&&) noexcept;

    functor& body();
    const functor& body() const;

    std::chrono::milliseconds& interval();
    const std::chrono::milliseconds& interval() const;

    bool& one_and_done();
    const bool& one_and_done() const;

    bool& run_immediately();
    const bool& run_immediately() const;

    boost::asio::steady_timer& timer();
    const boost::asio::steady_timer& timer() const;

    std::shared_ptr<operation_context_impl> impl;
};

operation_context make_operation_context();

}