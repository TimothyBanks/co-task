#include <tbb/task_arena.h>
#include <tbb/task_group.h>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <cotask/cotask_context.hpp>
#include <cotask/cotask_exception.hpp>
#include <cotask/execution_guard.hpp>
#include <cotask/this_thread.hpp>
#include <map>

namespace std {
template <>
struct less<cotask::operation_context> {
  bool operator()(const cotask::operation_context& lhs,
                  const cotask::operation_context& rhs) const {
    return lhs.address() < rhs.address();
  }
};
}  // namespace std

namespace cotask {

struct cotask_context_impl
    : public std::enable_shared_from_this<cotask_context_impl> {
  std::atomic<context_status> status = context_status::stopped;
  tbb::task_arena arena;
  tbb::task_group group;
  boost::asio::io_context io;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      work_guard;
  std::map<operation_context, std::shared_ptr<boost::asio::steady_timer>>
      timers;

  void start() {
    status = context_status::running;

    for (auto t : timers) {
      // Normally don't want a non const reference to a key.
      // But in this case, modifications to the key instance
      // will NOT affect the ordering within the map as the
      // the map works off the PIMPL address which will never change.
      auto& oc = const_cast<operation_context&>(t.first);
      schedule(oc);
    }

    io.run();
  }

  void stop() {
    status = context_status::stopped;

    for (auto it : timers) {
      it.second->cancel();
    }

    io.stop();
    group.wait();
  }

  void schedule(operation_context& op) {
    auto it = timers.find(op);

    if (it == std::end(timers)) {
      attach(op);
      // attach will re-enter this function.
      return;
    }

    if (op.run_immediately()) {
      op.run_immediately() = false;
      it->second->expires_after(op.interval());
    } else {
      it->second->expires_after(op.interval());
    }

    auto wp = std::weak_ptr<cotask_context_impl>(shared_from_this());
    it->second->async_wait([op, wp](const auto ec) {
      if (ec == boost::asio::error::operation_aborted) {
        return;
      }

      auto cc = wp.lock();
      if (!cc || cc->status == context_status::stopped) {
        return;
      }

      // execution_guard should protect against the rescheduling of this task
      // while it is executing.  We need to create the guard as soon as possible
      // and make sure to pass it along to the thread pool that will execute the
      // task.
      cc->arena.execute([cc, op]() {
        cc->group.run([cc, op]() {
         if (op.execution_count()) {
           return;
         }
         auto eg = execution_guard{cc, op};
         cotask::threading::this_thread::oc = op;
         cotask::threading::this_thread::cc = cc;
         op();
        });
      });
    });
  }

  void attach(operation_context& op) {
    auto it = timers.find(op);

    if (it != std::end(timers)) {
      detach(op);
    }

    auto timer = std::make_shared<boost::asio::steady_timer>(io);
    timers.emplace(op, std::move(timer));
    schedule(op);
  }

  void detach(operation_context& op) {
    auto it = timers.find(op);
    if (it == std::end(timers)) {
      return;
    }

    it->second->cancel();
    timers.erase(it);
  }

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

cotask_context::cotask_context(std::shared_ptr<cotask_context_impl> impl_)
    : impl{std::move(impl_)} {}

cotask_context& cotask_context::operator=(
    std::shared_ptr<cotask_context_impl> impl_) {
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

void cotask_context::schedule(operation_context& op) {
  if (!impl) {
    throw cotask_exception{"impl is invalid"};
  }
  return impl->schedule(op);
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