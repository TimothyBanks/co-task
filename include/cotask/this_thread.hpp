#pragma once
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>

namespace cotask::threading::this_thread {

thread_local cotask_context cc;
thread_local operation_context oc;

}  // namespace cotask::threading::this_thread
