#pragma once

namespace templates {
constexpr unsigned bits_per_char = 8;

template <class T>
consteval unsigned SizeInBits() {
  return sizeof(T) * bits_per_char;
}
}  // namespace templates
