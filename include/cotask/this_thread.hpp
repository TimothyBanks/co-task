#pragma once
#include <cotask/cotask_context.hpp>
#include <cotask/operation_context.hpp>

namespace cotask::threading::this_thread {

thread_local cotask_context main_context;
thread_local operation_context operation; 

}
