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

/**
 * @brief A simple thread pool implementation.
 *
 * This class manages a pool of worker threads to which tasks can be submitted.
 */
class ThreadPool {
 public:
  /**
   * @brief Constructs a ThreadPool and launches the specified number of worker
   * threads.
   *
   * @param threads The number of worker threads to launch.
   */
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

  /**
   * @brief Adds a new task to the pool.
   *
   * @tparam F The type of the function to be executed.
   * @tparam Args The types of the arguments to be passed to the function.
   * @param f The function to be executed.
   * @param args The arguments to be passed to the function.
   * @return A future that will hold the result of the function execution.
   */
  template <class F, class... Args>
  auto Enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    return EnqueueTask(std::move(task));
  }

  /**
   * @brief Adds a new task to the pool.
   *
   * @tparam T The type of the task.
   * @param task The task to be executed.
   * @return A future that will hold the result of the task execution.
   */
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

  /**
   * @brief Destructs the ThreadPool and joins all threads.
   */
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

 protected:
  std::vector<std::thread> workers;         ///< Vector of worker threads.
  std::queue<std::function<void()>> tasks;  ///< Queue of tasks.

  std::mutex
      queue_mutex;  ///< Mutex for synchronizing access to the task queue.
  std::condition_variable
      condition;  ///< Condition variable for task notification.
  bool stop;      ///< Flag indicating whether the pool is stopping.
};
