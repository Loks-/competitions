#pragma once

#include "common/base.h"

#include <mutex>
#include <vector>

namespace numeric {

/**
 * @brief A class to generate and manage Fibonacci numbers.
 *
 * This class provides methods to generate Fibonacci numbers up to a specified
 * index or value, and to retrieve Fibonacci numbers.
 *
 * @tparam T The type of the Fibonacci numbers.
 */
template <class T>
class TFibonacci {
 public:
  using TSelf = TFibonacci<T>;

 public:
  /**
   * @brief Constructor that initializes the Fibonacci sequence with the first
   * two numbers.
   */
  constexpr TFibonacci() : vf({T(0), T(1)}) {}

  /**
   * @brief Adjusts the Fibonacci sequence to include at least n elements.
   *
   * This function ensures that the Fibonacci sequence has at least n elements.
   * If the sequence is shorter than n, it will be extended.
   *
   * @param n The number of elements to ensure in the Fibonacci sequence.
   */
  constexpr void Adjust(unsigned n) const {
    if (vf.size() <= n) {
      const std::lock_guard<std::mutex> lock(m);
      AdjustI(n);
    }
  }

  /**
   * @brief Adjusts the Fibonacci sequence to include values up to a specified
   * value.
   *
   * This function ensures that the Fibonacci sequence includes values up to the
   * specified value. If the sequence does not include the value, it will be
   * extended.
   *
   * @param value The value to ensure in the Fibonacci sequence.
   */
  constexpr void AdjustToValue(const T& value) const {
    if (vf.back() < value) {
      const std::lock_guard<std::mutex> lock(m);
      AdjustToValueI(value);
    }
  }

  /**
   * @brief Returns the current size of the Fibonacci sequence.
   *
   * This function returns the current number of elements in the Fibonacci
   * sequence. Note that this function is marked as `const` but relies on a
   * mutable vector, meaning the output can change even if the instance is
   * `const`.
   *
   * @return The current size of the Fibonacci sequence.
   */
  constexpr size_t CurrentSize() const noexcept { return vf.size(); }

  /**
   * @brief Retrieves the Fibonacci number at the specified index.
   *
   * This function retrieves the Fibonacci number at the specified index. If the
   * Fibonacci sequence does not include the number, it will be extended.
   *
   * @param n The index of the Fibonacci number to retrieve.
   * @return The Fibonacci number at the specified index.
   */
  constexpr T Get(unsigned n) const {
    Adjust(n);
    return vf[n];
  }

  /**
   * @brief Operator overload to retrieve the Fibonacci number at the specified
   * index.
   *
   * This operator overload allows the class to be called like a function to
   * retrieve the Fibonacci number at the specified index.
   *
   * @param n The index of the Fibonacci number to retrieve.
   * @return The Fibonacci number at the specified index.
   */
  constexpr T operator()(unsigned n) const { return Get(n); }

 public:
  /**
   * @brief Retrieves a static instance of the Fibonacci class.
   *
   * This function returns a static instance of the Fibonacci class.
   *
   * @return A static instance of the Fibonacci class.
   */
  static const TSelf& GetS() {
    static const TSelf f;
    return f;
  }

  /**
   * @brief Retrieves the Fibonacci number at the specified index using a static
   * instance.
   *
   * This function returns the Fibonacci number at the specified index using a
   * static instance of the Fibonacci class.
   *
   * @param n The index of the Fibonacci number to retrieve.
   * @return The Fibonacci number at the specified index.
   */
  static T SGet(unsigned n) { return GetS().Get(n); }

 protected:
  /**
   * @brief Internal function to adjust the Fibonacci sequence to include at
   * least n elements.
   *
   * This function extends the Fibonacci sequence to include at least n
   * elements.
   *
   * @param n The number of elements to ensure in the Fibonacci sequence.
   */
  constexpr void AdjustI(unsigned n) const {
    for (auto i = unsigned(vf.size()); i <= n; ++i)
      vf.push_back(vf[i - 2] + vf[i - 1]);
  }

  /**
   * @brief Internal function to adjust the Fibonacci sequence to include values
   * up to a specified value.
   *
   * This function extends the Fibonacci sequence to include values up to the
   * specified value.
   *
   * @param value The value to ensure in the Fibonacci sequence.
   */
  constexpr void AdjustToValueI(const T& value) const {
    for (auto i = vf.size() - 1; vf[i] < value; ++i)
      vf.push_back(vf[i - 1] + vf[i]);
  }

 protected:
  mutable std::mutex m;  ///< Mutex to protect access to the Fibonacci sequence.
  mutable std::vector<T> vf;  ///< Vector to store the Fibonacci sequence.
};

using UFibonacci = TFibonacci<uint64_t>;

}  // namespace numeric
