#pragma once

#include "common/base.h"
#include "common/template.h"
#include "common/templates/bits.h"

namespace nhash {
// From boost 1.86
constexpr uint32_t MixI(uint32_t x, TFakeUnsigned<32>) {
  x ^= x >> 16;
  x *= 0x21f0aaad;
  x ^= x >> 15;
  x *= 0x735a2d97;
  x ^= x >> 15;
  return x;
}

// // https://jonkagstrom.com/mx3/mx3_rev2.html
// constexpr uint64_t MixI(uint64_t x, TFakeUnsigned<64>) {
//   x ^= x >> 32;
//   x *= 0xe9846af9b1a615d;
//   x ^= x >> 32;
//   x *= 0xe9846af9b1a615d;
//   x ^= x >> 28;
//   return x;
// }

// https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
constexpr uint64_t MixI(uint64_t x, TFakeUnsigned<64>) {
  x ^= x >> 27;
  x *= 0x3C79AC492BA7B653UL;
  x ^= x >> 33;
  x *= 0x1C69B3F74AC4AE35UL;
  x ^= x >> 27;
  return x;
}

constexpr size_t Mix(size_t x) {
  return MixI(x, TFakeUnsigned<templates::SizeInBits<size_t>()>{});
}
}  // namespace nhash
