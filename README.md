# cotask
A task based framework that uses co-routines for managing blocking operations within a task based paradigm.  Tasks can be recurring or one off.  When a blocking operation occurs, the current task should co_return and allow the executing thread to process another task in the queue.

This SDK makes use of oneTBB for task based programming and Boost ASIO for asynchronous operations.  As stated the basic idea is to employ co-routines to bridge the gap between the need for blocking operations such as IO or trying to acquire a mutex with task based programming and its need for allowing no blocking calls.  Co-routines will be used for allowing the task to return from an executing task body when it encounters a blocking call, create an awaitable to check on the blocking operation over time and when the blocking operation has successfully completed, continue on with the rest of the task operation.

As usual a code example is a good way to demonstrate this idea.  The main point to identify here is the blocking call when trying to acquire a mutex on every task invocation.  The mutex acquisition is sitting within an awaitable that will be polled periodically to see if the lock can actually be acquired.

```
struct acquire_lock_awaitable : cotask::basic_awaitable {
  std::unique_lock<std::mutex> guard;
  std::function<void(void)> reset_promise;
  std::coroutine_handle<> parent;

  acquire_lock_awaitable() = delete;
  acquire_lock_awaitable(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable(acquire_lock_awaitable&&) = default;
  acquire_lock_awaitable(std::unique_lock<std::mutex> guard_)
      : guard{std::move(guard_)} {}

  acquire_lock_awaitable& operator=(const acquire_lock_awaitable&) = delete;
  acquire_lock_awaitable& operator=(acquire_lock_awaitable&&) = default;

  bool await_ready() { return guard.try_lock(); }

  template <typename T>
  void await_suspend(std::coroutine_handle<T> h) {
    set_ready_functor(h, [&]() { return guard.try_lock(); });
    parent = h;
  }

  void await_resume() { reset_handle(); }
};

cotask::task<void> foobar() {
  std::cout << "foobar is invoked from thread " << std::this_thread::get_id()
            << std::endl;
  co_await std::suspend_always{};
  std::cout << "foobar is is back from suspension from thread "
            << std::this_thread::get_id() << std::endl;
  co_return;
}

std::mutex write_mutex;

std::function<cotask::any_task(void)> make_task(
    std::string id,
    std::chrono::milliseconds timeout) {
  return [id = std::move(id), timeout]() -> cotask::task<void> {
    auto lock_awaitable = acquire_lock_awaitable{
        std::unique_lock<std::mutex>{write_mutex, std::defer_lock}};
    co_await lock_awaitable;
    std::this_thread::sleep_for(timeout);
    std::cout << "hello from me " << id << " thread id "
              << std::this_thread::get_id() << std::endl;
    auto fb = foobar();
    co_await cotask::awaitable{fb};
    std::cout << "hello from me after suspension " << id << " thread id "
              << std::this_thread::get_id() << std::endl;
    co_await std::suspend_always{};
    std::cout << "hello from me after another suspension " << id
              << " thread id " << std::this_thread::get_id() << std::endl;
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

  auto cc = cotask::make_cotask_context();
  for (auto& oc : ocs) {
    cc.attach(oc);
  }
  cc.start();
}
```
