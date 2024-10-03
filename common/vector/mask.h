#pragma once

#include <type_traits>
#include <vector>

namespace nvector {

/**
 * @brief Converts a bitmask to a vector of set bit positions.
 *
 * This function takes a bitmask and returns a vector containing the positions
 * of the set bits (1s) in the bitmask.
 *
 * @tparam T The type of the bitmask (e.g., uint8_t, uint16_t, uint32_t,
 * uint64_t).
 * @param mask The bitmask to convert.
 * @return A vector containing the positions of the set bits in the bitmask.
 */
template <typename T>
constexpr std::vector<unsigned> MaskToVector(T mask) {
  static_assert(std::is_integral<T>::value, "T must be an integral type.");
  std::vector<unsigned> v;
  for (unsigned i = 0; mask; ++i) {
    T b = (T(1) << i);
    if (mask & b) {
      v.push_back(i);
      mask &= ~b;
    }
  }
  return v;
}

}  // namespace nvector
