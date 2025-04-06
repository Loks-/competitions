#pragma once

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Exception class for assertion failures.
 */
class AssertException : public std::logic_error {
 public:
  AssertException(const std::string& message) : std::logic_error(message) {}
};

/**
 * @brief Asserts that a condition is true and throws an exception if it fails.
 *
 * @param condition The condition to check.
 * @throws AssertException if the condition is false.
 */
constexpr void assert_exception(bool condition) {
  if (!condition) throw AssertException("");
}

/**
 * @brief Asserts that a condition is true and throws an exception if it fails,
 * with a custom message.
 *
 * @param condition The condition to check.
 * @param message The message to display if the assertion fails.
 * @throws AssertException if the condition is false.
 */
constexpr void assert_exception(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << std::format("Assertion failed: {}\n", message);
    throw AssertException(message);
  }
}

/**
 * @brief Asserts that a condition is true and throws an exception if it fails,
 * with a formatted message.
 *
 * @param condition The condition to check.
 * @param format The format string.
 * @param args The format arguments.
 * @throws AssertException if the condition is false.
 */
template <typename... Args>
constexpr void assert_exception(bool condition,
                                std::format_string<Args...> format,
                                Args&&... args) {
  if (!condition) {
    const std::string message =
        std::format(format, std::forward<Args>(args)...);
    std::cerr << std::format("Assertion failed: {}\n", message);
    throw AssertException(message);
  }
}
