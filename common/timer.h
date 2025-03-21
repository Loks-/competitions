#pragma once

#include <chrono>

/**
 * @brief A modern timer class for measuring elapsed time.
 *
 * This class provides functionality to start, stop, and measure the elapsed
 * time in various units. It uses steady_clock for more accurate timing
 * measurements.
 */
class Timer {
 public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using Duration = std::chrono::nanoseconds;

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
    start_time = Clock::now();
    running = true;
  }

  /**
   * @brief Stops the timer.
   */
  void Stop() {
    end_time = Clock::now();
    running = false;
  }

  /**
   * @brief Resets the timer to its initial state.
   */
  void Reset() {
    running = false;
    start_time = TimePoint{};
    end_time = TimePoint{};
  }

  /**
   * @brief Gets the current elapsed duration.
   *
   * @return The elapsed duration in nanoseconds.
   */
  [[nodiscard]] Duration GetDuration() const {
    auto time = running ? Clock::now() : end_time;
    return time - start_time;
  }

  /**
   * @brief Gets the elapsed time in nanoseconds.
   *
   * @return The elapsed time in nanoseconds.
   */
  [[nodiscard]] size_t GetNanoseconds() const { return GetDuration().count(); }

  /**
   * @brief Gets the elapsed time in microseconds.
   *
   * @return The elapsed time in microseconds.
   */
  [[nodiscard]] size_t GetMicroseconds() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(GetDuration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in milliseconds.
   *
   * @return The elapsed time in milliseconds.
   */
  [[nodiscard]] size_t GetMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(GetDuration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in seconds.
   *
   * @return The elapsed time in seconds.
   */
  [[nodiscard]] size_t GetSeconds() const {
    return std::chrono::duration_cast<std::chrono::seconds>(GetDuration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in minutes.
   *
   * @return The elapsed time in minutes.
   */
  [[nodiscard]] size_t GetMinutes() const {
    return std::chrono::duration_cast<std::chrono::minutes>(GetDuration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in hours.
   *
   * @return The elapsed time in hours.
   */
  [[nodiscard]] size_t GetHours() const {
    return std::chrono::duration_cast<std::chrono::hours>(GetDuration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in a specified unit.
   *
   * @tparam T The duration type to convert to.
   * @return The elapsed time in the specified unit.
   */
  template <typename T>
  [[nodiscard]] typename T::rep GetAs() const {
    return std::chrono::duration_cast<T>(GetDuration()).count();
  }

  /**
   * @brief Checks if the timer is currently running.
   *
   * @return true if the timer is running, false otherwise.
   */
  [[nodiscard]] bool IsRunning() const noexcept { return running; }

 protected:
  bool running;          ///< Indicates whether the timer is currently running.
  TimePoint start_time;  ///< The time when the timer was started.
  TimePoint end_time;    ///< The time when the timer was stopped.
};
