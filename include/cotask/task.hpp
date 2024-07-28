#pragma once
#include <coroutine>

namespace cotask {

template <typename T>
struct task {
    template<typename T>
    struct promise {
        std::optional<std::conditonal_t<std::is_void_v<T>, bool, T>> value;
        std::exception_ptr exception;

        std::suspend_always initial_suspend() { 
            return {}; 
        }
        
        std::suspend_never final_suspend() noexcept { 
            return {}; 
        }

        void unhandled_exception() {
            exception = std::current_exception();
        }

        T result() requires (!std::is_void_v<T>) {
            return value.value_or(T{});
        }

        void return_value(T v) requires (!std::is_void_v<T>) {
            value = std::move(v);
        }

        void return_void() requires std::is_void_v<T> {}

        task<T> get_return_object() {
            return task<T>{ std::coroutine_handle<promise<T>>::from_promise(*this) };
        }
    };
};

}