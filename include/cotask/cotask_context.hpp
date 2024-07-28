#pragma once
#include <atomic>
#include <cotask/operation_context.hpp>
#include <memory>

namespace cotask {

struct cotask_context_impl;

enum class context_status {
    running,
    stopped
};

struct cotask_context {
    std::atomic<context_status>& status();
    const std::atomic<context_status>& status() const;

    cotask_context() noexcept;
    cotask_context(std::shared_ptr<cotask_context_impl> impl);
    cotask_context(const cotask_context&) noexcept;
    cotask_context(cotask_context&&) noexcept;

    cotask_context& operator=(const cotask_context&) noexcept;
    cotask_context& operator=(std::shared_ptr<cotask_context_impl> impl);
    cotask_context& operator=(cotask_context&&) noexcept;

    void start();
    void stop();

    void attach(operation_context& op);
    void schedule(operation_context& op);
    void detach(operation_context& op);

    std::shared_ptr<cotask_context_impl> impl;
};

cotask_context make_cotask_context();

}