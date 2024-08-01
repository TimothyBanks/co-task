#include <cotask/execution_guard.hpp>

using namespace cotask;

execution_guard::execution_guard(cotask_context cc_, operation_context oc_)
    : cc{std::move(cc_)}, oc{[&]() {
        ++oc_.execution_count();
        return std::move(oc_);
      }()} {}

execution_guard::~execution_guard() {
  if (--oc.execution_count() > 0) {
    return;
  }

  if (oc.one_and_done()) {
    cc.detach(oc);
    return;
  }

  if (cc.status() == context_status::stopped) {
    return;
  }

  // restart the timer
  cc.schedule(oc);
}