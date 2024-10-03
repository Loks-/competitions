#pragma once

#include <iostream>
#include <string>

/**
 * @brief Exception class for assertion failures.
 */
class AssertException {};

/**
 * @brief Asserts that a condition is true.
 *
 * @param condition The condition to check.
 * @throws AssertException if the condition is false.
 */
inline void Assert(bool condition) {
  if (!condition) {
    throw AssertException();
  }
}

/**
 * @brief Asserts that a condition is true, with a custom message.
 *
 * @param condition The condition to check.
 * @param message The message to display if the assertion fails.
 * @throws AssertException if the condition is false.
 */
inline void Assert(bool condition, const std::string& message) {
  if (!condition) {
    std::cerr << "Exception: " << message << std::endl;
    throw AssertException();
  }
}
