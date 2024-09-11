#pragma once

namespace templates {

// Number of bits in a single character (char)
constexpr unsigned bits_per_char = 8;

/**
 * @brief Calculates the size of a type in bits.
 *
 * This function template calculates the size of the given type T in bits.
 * It uses the constexpr value bits_per_char to determine the number of bits per
 * character.
 *
 * @tparam T The type whose size in bits is to be calculated.
 * @return The size of the type T in bits.
 */
template <class T>
consteval unsigned SizeInBits() {
  return sizeof(T) * bits_per_char;
}

}  // namespace templates
