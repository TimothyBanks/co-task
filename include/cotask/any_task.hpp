#pragma once
#include <cotask/task.hpp>
#include <memory>

namespace cotask {

///
/// \brief A type erasure type for task templates.
///
struct any_task {
 private:
  struct task_interface {
    virtual ~task_interface() = default;

    virtual void resume() = 0;
    virtual bool done() const = 0;
  };

  template <typename T>
  struct task_impl : task_interface {
    task<T> t_;

    task_impl() = default;
    task_impl(const task_impl&) = default;
    task_impl(task_impl&&) = default;
    task_impl(task<T> t) : t_{std::move(t)} {}
    ~task_impl() override = default;

    task_impl& operator=(const task_impl&) = default;
    task_impl& operator=(task_impl&&) = default;

    virtual void resume() override { t_.resume(); }

    virtual bool done() const override { return t_.done(); }
  };

  std::unique_ptr<task_interface> t_;

 public:
  any_task() = default;
  any_task(const any_task&) = delete;
  any_task(any_task&&) = default;

  template <typename T>
  any_task(task<T> t) : t_{std::make_unique<task_impl<T>>(std::move(t))} {}

  any_task& operator=(const any_task&) = delete;
  any_task& operator=(any_task&&) = default;

  void resume() {
    if (!t_) {
      return;
    }
    t_->resume();
  }

  bool done() const {
    if (!t_) {
      return true;
    }
    return t_->done();
  }
};

}  // namespace cotask