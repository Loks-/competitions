#pragma once

#include <chrono>

class Timer {
 protected:
  bool running;
  std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

 public:
  void Start() {
    start_time = std::chrono::system_clock::now();
    running = true;
  }

  void Stop() {
    end_time = std::chrono::system_clock::now();
    running = false;
  }

  explicit Timer(bool start = true) : running(false) {
    if (start) Start();
  }

  size_t GetMicroseconds() const {
    std::chrono::time_point<std::chrono::system_clock> time =
        running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::microseconds>(time -
                                                                 start_time)
        .count();
  }

  size_t GetMilliseconds() const {
    std::chrono::time_point<std::chrono::system_clock> time =
        running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::milliseconds>(time -
                                                                 start_time)
        .count();
  }

  size_t GetSeconds() const {
    std::chrono::time_point<std::chrono::system_clock> time =
        running ? std::chrono::system_clock::now() : end_time;
    return std::chrono::duration_cast<std::chrono::seconds>(time - start_time)
        .count();
  }
};
