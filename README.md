# cotask
A task based framework that uses co-routines for managing blocking operations within a task based paradigm.  Tasks can be recurring or one off.  When a blocking operation occurs, the current task should co_return and allow the executing thread to process another task in the queue.

This SDK makes use of oneTBB for task based programming and Boost ASIO for asynchronous operations.  As stated the basic idea is to employ co-routines to bridge the gap between the need for blocking operations such as IO or trying to acquire a mutex with task based programming and its need for allowing no blocking calls.  Co-routines will be used for allowing the task to return from an executing task body when it encounters a blocking call, create an awaitable to check on the blocking operation over time and when the blocking operation has successfully completed, continue on with the rest of the task operation.

As usual a code example is a good way to demonstrate this idea.  The main point to identify here is the blocking call when trying to acquire a mutex on every task invocation.  The mutex acquisition is sitting within an awaitable that will be polled periodically to see if the lock can actually be acquired.

```
cotask::cotask_context cc = cotask::make_cotask_context();
std::unique_ptr<std::mutex> write_mutex = std::make_unique<std::mutex>();

struct task_awaitable : cotask::basic_awaitable {
  cotask::operation_context op;
  bool completed{false};

  task_awaitable() = delete;
  task_awaitable(const task_awaitable&) = delete;
  task_awaitable(task_awaitable&&) = default;

  template <typename Task_functor>
  task_awaitable(Task_functor f)
      : op{cotask::make_operation_context(
            [f = std::move(f), this]() -> cotask::task<void> {
              f();
              completed = true;
              co_return;
            },
            std::chrono::milliseconds{1500},
            true,
            true)} {
    cc.attach(op);
  }

  task_awaitable& operator=(const task_awaitable&) = delete;
  task_awaitable& operator=(task_awaitable&&) = default;

  bool await_ready() { return completed; }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() { return completed; });
  }

  void await_resume() { reset_handle(); }
};

struct async_awaitable : cotask::basic_awaitable {
  bool completed{false};

  async_awaitable() = delete;
  async_awaitable(const async_awaitable&) = delete;
  async_awaitable(async_awaitable&&) = default;

  template <typename Thread_functor>
  async_awaitable(Thread_functor f) {
    auto h = std::async(std::launch::async, [f = std::move(f), this]() {
        f();
        completed = true;
    });
  }

  async_awaitable& operator=(const async_awaitable&) = delete;
  async_awaitable& operator=(async_awaitable&&) = default;

  bool await_ready() { return completed; }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() { return completed; });
  }

  void await_resume() { reset_handle(); }
};

struct acquire_lock_awaitable : cotask::basic_awaitable {
  std::unique_lock<std::mutex> guard;

  acquire_lock_awaitable() = delete;
  acquire_lock_awaitable(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable(acquire_lock_awaitable&&) = default;
  acquire_lock_awaitable(std::unique_lock<std::mutex> guard_)
      : guard{std::move(guard_)} {}

  acquire_lock_awaitable& operator=(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable& operator=(acquire_lock_awaitable&&) = default;

  bool await_ready() {
    auto ready = guard.try_lock();
    // std::cout << "[" << std::this_thread::get_id() << "] Lock acquired == " << ready << std::endl;
    return ready;
  }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_awaiting_handle(h, [&]() {
      auto ready = guard.try_lock();
      // std::cout << "[" << std::this_thread::get_id() << "] Lock acquired == " << ready << std::endl;
      return ready;
    });
  }

  void await_resume() { reset_handle(); }
};

cotask::task<void> foobar(const std::string& id) {
  std::cout << "[" << id << ", " << std::this_thread::get_id() << "] foobar invoked." << std::endl;
  co_await std::suspend_always{};
  std::cout << "[" << id << ", " << std::this_thread::get_id() << "] foobar after suspension." << std::endl;
  co_return;
}

std::function<cotask::any_task(void)> make_task(
    std::string id,
    std::chrono::milliseconds timeout) {
  return [id = std::move(id), timeout]() -> cotask::task<void> {
    auto lock_awaitable = acquire_lock_awaitable{
        std::unique_lock<std::mutex>{*write_mutex, std::defer_lock}};
    co_await lock_awaitable;
    std::cout << "[" << id << ", " << std::this_thread::get_id() << "] Lock acquired. Going to sleep." << std::endl;
    std::this_thread::sleep_for(timeout / 2);

    auto tid = std::this_thread::get_id();
    auto async_await = async_awaitable{[&] {
      std::cout << "[" << id << ", " << std::this_thread::get_id() << "] Ran asynchronous operation. " << std::endl;
    }};
    co_await async_await;

    auto fb = foobar(id);
    co_await cotask::awaitable{fb};
    std::cout << "[" << id << ", " << std::this_thread::get_id() << "] Coming back after first suspension " << std::endl;
    co_await std::suspend_always{};
    std::cout << "[" << id << ", " << std::this_thread::get_id() << "] Coming back after last suspension " << std::endl;
    co_return;
  };
}

BOOST_AUTO_TEST_CASE(cotask_tests) {
  auto i = size_t{0};

  auto ocs = std::vector<cotask::operation_context>{
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
      cotask::make_operation_context(
          make_task(std::to_string(++i), std::chrono::milliseconds{1000}),
          std::chrono::milliseconds{1}, false, false),
  };

  for (auto& oc : ocs) {
    cc.attach(oc);
  }
  cc.start();
}
```
