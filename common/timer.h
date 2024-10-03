#pragma once

#include <chrono>

/**
 * @brief A simple timer class for measuring elapsed time.
 *
 * This class provides functionality to start, stop, and measure the elapsed
 * time in microseconds, milliseconds, and seconds.
 */
class Timer {
 public:
  /**
   * @brief Constructs a Timer object.
   *
   * @param start If true, the timer starts immediately upon construction.
   */
  explicit Timer(bool start = true) : running(false) {
    if (start) {
      Start();
    }
  }

  /**
   * @brief Starts the timer.
   */
  void Start() {
    start_time = std::chrono::system_clock::now();
    running = true;
  }

  /**
   * @brief Stops the timer.
   */
  void Stop() {
    end_time = std::chrono::system_clock::now();
    running = false;
  }

  /**
   * @brief Gets the elapsed time in microseconds.
   *
   * @return The elapsed time in microseconds.
   */
  size_t GetMicroseconds() const {
    auto time = running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::microseconds>(time -
                                                                 start_time)
        .count();
  }

  /**
   * @brief Gets the elapsed time in milliseconds.
   *
   * @return The elapsed time in milliseconds.
   */
  size_t GetMilliseconds() const {
    auto time = running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::milliseconds>(time -
                                                                 start_time)
        .count();
  }

  /**
   * @brief Gets the elapsed time in seconds.
   *
   * @return The elapsed time in seconds.
   */
  size_t GetSeconds() const {
    auto time = running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::seconds>(time - start_time)
        .count();
  }

 protected:
  bool running;  ///< Indicates whether the timer is currently running.
  std::chrono::time_point<std::chrono::system_clock> start_time, end_time;
};
