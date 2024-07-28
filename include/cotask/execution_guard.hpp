#pragma once
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>

namespace cotask {

struct execution_guard {
    cotask_context cc;
    operation_context oc;

    execution_guard() noexcept = default;
    execution_guard(const execution_guard&) = delete;
    execution_guard(execution_guard&&) noexcept = default;
    execution_guard(cotask_context cc, operation_context oc);
    ~execution_guard();

    execution_guard& operator=(const execution_guard&) = delete;
    execution_guard& operator=(execution_guard&&) noexcept = default;
};

}