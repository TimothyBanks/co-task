#include <tbb/tbb.h>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/cotask_exception.hpp>

namespace cotask {

struct cotask_context_impl : public std::enable_shared_from_this<cotask_context_impl> {
  std::atomic<context_status> status = context_status::stopped;
  tbb::task_arena arena;
  tbb::task_group group;
  boost::asio::io_context io;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      work_guard;

  void start() {}
  void stop() {}

  void attach(operation_context& op) {
    if (op.timer()) {
        detach(op);
    }

    op.timer() = std::make_unique<boost::asio::steady_timer>(io);
    op.timer()->expires_after(op.interval());

    auto wp = std::weak_ptr<cotask_context_impl>(shared_from_this());
    op.timer()->async_wait([op, wp](const auto ec){
        auto cc = wp.lock();
        if (!cc || cc->status == context_status::stopped) {
            // Tear down this callback
        }


    });
  }

  void detach(operation_context& op) {}

  cotask_context_impl() : work_guard{io.get_executor()} {}
};

cotask_context make_cotask_context() {
  auto context = cotask_context{};
  context.impl = std::make_shared<cotask_context_impl>();
  return context;
}

cotask_context::cotask_context() noexcept = default;
cotask_context::cotask_context(const cotask_context&) noexcept = default;
cotask_context::cotask_context(cotask_context&&) noexcept = default;

cotask_context& cotask_context::operator=(const cotask_context&) noexcept =
    default;
cotask_context& cotask_context::operator=(cotask_context&&) noexcept = default;

cotask_context::cotask_context(std::shared_ptr<cotask_context_impl> impl_) : impl{std::move(impl_)} {}

cotask_context& cotask_context::operator=(std::shared_ptr<cotask_context_impl> impl_) {
    impl = std::move(impl_);
    return *this;
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

void cotask_context::attach(operation_context& op) {
  if (!impl) {
    throw cotask_exception{"impl is invalid"};
  }
  return impl->attach(op);
}

void cotask_context::detach(operation_context& op) {
  if (!impl) {
    throw cotask_exception{"impl is invalid"};
  }
  return impl->detach(op);
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

}  // namespace cotask