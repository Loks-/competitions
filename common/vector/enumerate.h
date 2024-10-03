#pragma once

#include <vector>

namespace nvector {

/**
 * @brief Generates a vector of values from begin to end with a default step
 * of 1.
 *
 * This function creates a std::vector containing a sequence of values starting
 * from `begin` and ending before `end`, with a default step value of 1.
 *
 * @tparam TValue The type of the values in the vector.
 * @param begin The starting value of the sequence.
 * @param end The value at which to stop the sequence (exclusive).
 * @return A std::vector containing the generated sequence of values.
 */
template <class TValue>
constexpr std::vector<TValue> Enumerate(TValue begin, TValue end) {
  std::vector<TValue> output;
  for (; begin < end;) {
    output.push_back(begin++);
  }
  return output;
}

/**
 * @brief Generates a vector of values from begin to end with a specified step.
 *
 * This function creates a std::vector containing a sequence of values starting
 * from `begin`, incremented by `step`, and ending before `end`.
 *
 * @tparam TValue The type of the values in the vector.
 * @param begin The starting value of the sequence.
 * @param end The value at which to stop the sequence (exclusive).
 * @param step The step value by which to increment the sequence.
 * @return A std::vector containing the generated sequence of values.
 */
template <class TValue>
constexpr std::vector<TValue> Enumerate(TValue begin, TValue end, TValue step) {
  std::vector<TValue> output;
  for (; begin < end; begin += step) {
    output.push_back(begin);
  }
  return output;
}

}  // namespace nvector
