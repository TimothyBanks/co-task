#include <cotask/cotask_exception.hpp>
#include <cotask/operation_context.hpp>

namespace cotask {

struct operation_context_impl
    : public std::enable_shared_from_this<operation_context_impl> {
  std::function<void(void)> body;
  std::chrono::milliseconds interval;
  bool one_and_done{false};
  bool run_immediately{false};
  std::unique_ptr<boost::asio::steady_timer> timer;
};

operation_context make_operation_context(operation_context::functor body,
                                         std::chrono::milliseconds interval,
                                         bool one_and_done,
                                         bool run_immediately) {
  auto context = operation_context{};
  context.impl = std::make_shared<operation_context_impl>();
  context.impl->body = std::move(body);
  context.impl->interval = std::move(interval);
  context.impl->one_and_done = one_and_done;
  context.impl->run_immediately = run_immediately;
  return context;
}

operation_context::operation_context() noexcept = default;
operation_context::operation_context(const operation_context&) noexcept =
    default;
operation_context::operation_context(operation_context&&) noexcept = default;

operation_context& operation_context::operator=(
    const operation_context&) noexcept = default;
operation_context& operation_context::operator=(operation_context&&) noexcept =
    default;

operation_context::operation_context(std::shared_ptr<operation_context_impl> impl_) : impl{std::move(impl_)} {}

operation_context& operation_context::operator=(std::shared_ptr<operation_context_impl> impl_) {
    impl = std::move(impl_);
    return *this;
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
  if (!impl || !impl->timer) {
    throw cotask_exception{"impl is invalid"};
  }
  return *(impl->timer);
}

const boost::asio::steady_timer& operation_context::timer() const {
  if (!impl || !impl->timer) {
    throw cotask_exception{"impl is invalid"};
  }
  return *(impl->timer);
}

}  // namespace cotask
