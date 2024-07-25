#pragma once
#include <atomic>
#include <boost/asio.hpp>
#include <memory>
#include <tbb/tbb.h>

namespace cotask {

struct cotask_context_impl;

enum class context_status {
    running,
    stopped
};

struct cotask_context {
    std::atomic<context_status>& status();
    const std::atomic<context_status>& status() const;

    boost::asio::io_context& io();
    const boost::asio::io_context& io() const;

    tbb::task_arena& arena();
    const tbb::task_arena& arena() const;

    tbb::task_group& group();
    const tbb::task_group& group() const;

    void start();
    void stop();

    std::shared_ptr<cotask_context_impl> impl;
};

cotask_context make_cotask_context();

}