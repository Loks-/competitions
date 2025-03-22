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
   * @param auto_start If true, the timer starts immediately upon construction.
   */
  explicit Timer(bool auto_start = true) : running_(false) {
    if (auto_start) {
      start();
    }
  }

  /**
   * @brief Starts the timer.
   */
  void start() {
    start_time_ = Clock::now();
    running_ = true;
  }

  /**
   * @brief Stops the timer.
   */
  void stop() {
    end_time_ = Clock::now();
    running_ = false;
  }

  /**
   * @brief Resets the timer to its initial state.
   */
  void reset() {
    running_ = false;
    start_time_ = TimePoint{};
    end_time_ = TimePoint{};
  }

  /**
   * @brief Gets the current elapsed duration.
   *
   * @return The elapsed duration in nanoseconds.
   */
  [[nodiscard]] Duration get_duration() const {
    auto time = running_ ? Clock::now() : end_time_;
    return time - start_time_;
  }

  /**
   * @brief Gets the elapsed time in nanoseconds.
   *
   * @return The elapsed time in nanoseconds.
   */
  [[nodiscard]] size_t get_nanoseconds() const {
    return get_duration().count();
  }

  /**
   * @brief Gets the elapsed time in microseconds.
   *
   * @return The elapsed time in microseconds.
   */
  [[nodiscard]] size_t get_microseconds() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(get_duration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in milliseconds.
   *
   * @return The elapsed time in milliseconds.
   */
  [[nodiscard]] size_t get_milliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(get_duration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in seconds.
   *
   * @return The elapsed time in seconds.
   */
  [[nodiscard]] size_t get_seconds() const {
    return std::chrono::duration_cast<std::chrono::seconds>(get_duration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in minutes.
   *
   * @return The elapsed time in minutes.
   */
  [[nodiscard]] size_t get_minutes() const {
    return std::chrono::duration_cast<std::chrono::minutes>(get_duration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in hours.
   *
   * @return The elapsed time in hours.
   */
  [[nodiscard]] size_t get_hours() const {
    return std::chrono::duration_cast<std::chrono::hours>(get_duration())
        .count();
  }

  /**
   * @brief Gets the elapsed time in a specified unit.
   *
   * @tparam DurationType The duration type to convert to.
   * @return The elapsed time in the specified unit.
   */
  template <typename DurationType>
  [[nodiscard]] typename DurationType::rep get_as() const {
    return std::chrono::duration_cast<DurationType>(get_duration()).count();
  }

  /**
   * @brief Checks if the timer is currently running.
   *
   * @return true if the timer is running, false otherwise.
   */
  [[nodiscard]] bool is_running() const noexcept { return running_; }

 protected:
  bool running_;          ///< Indicates whether the timer is currently running.
  TimePoint start_time_;  ///< The time when the timer was started.
  TimePoint end_time_;    ///< The time when the timer was stopped.
};
