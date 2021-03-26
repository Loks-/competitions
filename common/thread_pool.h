#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  // the constructor just launches some amount of workers
  explicit ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i)
      workers.emplace_back([this] {
        for (;;) {
          std::function<void()> task;

          {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty()) {
              return;
            }
            task = std::move(this->tasks.front());
            this->tasks.pop();
          }

          task();
        }
      });
  }

  // add new work item to the pool
  template <class F, class... Args>
  auto Enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    return EnqueueTask(std::move(task));
  }

  // add new work item to the pool
  template <class T>
  auto EnqueueTask(std::shared_ptr<std::packaged_task<T()>>&& task)
      -> std::future<T> {
    std::future<T> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);

      // don't allow enqueueing after stopping the pool
      if (stop) {
        throw std::runtime_error("Enqueue on stopped ThreadPool");
      }

      tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
  }

  // the destructor joins all threads
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
      worker.join();
    }
  }

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers;
  // the task queue
  std::queue<std::function<void()>> tasks;

  // synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};
