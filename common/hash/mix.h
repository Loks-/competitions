#pragma once

#include "common/base.h"
#include "common/template.h"
#include "common/templates/bits.h"

namespace nhash {

/**
 * @brief Mix function for 32-bit unsigned integers.
 *
 * This function applies a series of bitwise operations and multiplications to
 * mix the bits of a 32-bit unsigned integer. See boost 1.86 for more
 * information.
 *
 * @param x The 32-bit unsigned integer to be mixed.
 * @return The mixed 32-bit unsigned integer.
 */
constexpr uint32_t MixI(uint32_t x, MetaUnsigned<32>) {
  x ^= x >> 16;
  x *= 0x21f0aaad;
  x ^= x >> 15;
  x *= 0x735a2d97;
  x ^= x >> 15;
  return x;
}

/**
 * @brief Mix function for 64-bit unsigned integers.
 *
 * This function applies a series of bitwise operations and multiplications to
 * mix the bits of a 64-bit unsigned integer. See
 * https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
 * for more information.
 *
 * @param x The 64-bit unsigned integer to be mixed.
 * @return The mixed 64-bit unsigned integer.
 */
constexpr uint64_t MixI(uint64_t x, MetaUnsigned<64>) {
  x ^= x >> 27;
  x *= 0x3C79AC492BA7B653UL;
  x ^= x >> 33;
  x *= 0x1C69B3F74AC4AE35UL;
  x ^= x >> 27;
  return x;
}

/**
 * @brief General mix function for size_t.
 *
 * This function determines the size of size_t at compile time and applies the
 * appropriate MixI function.
 *
 * @param x The size_t value to be mixed.
 * @return The mixed size_t value.
 */
constexpr size_t Mix(size_t x) {
  return MixI(x, MetaUnsigned<templates::SizeInBits<size_t>()>{});
}

}  // namespace nhash
