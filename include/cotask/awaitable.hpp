#pragma once

namespace cotask {

struct awaitable {
    bool await_ready() const noexcept { return false; } 
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        // std::cout << "Suspended\n";
        // h.resume(); 
    }
    void await_resume() const noexcept { std::cout << "Resumed\n"; } /
};

}