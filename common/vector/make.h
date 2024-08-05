#pragma once

#include <vector>

namespace nvector {
namespace hidden {
template <class T, unsigned d>
class HDVector;

template <class T>
class HDVector<T, 0u> {
 public:
  using TType = T;
};

template <class T, unsigned d>
class HDVector {
 public:
  using TType = std::vector<typename HDVector<T, d - 1>::TType>;
};
}  // namespace hidden

template <class T, unsigned d>
using DVector = typename hidden::HDVector<T, d>::TType;

template <class T>
constexpr auto Make(unsigned d0, const T& value) {
  return DVector<T, 1>(d0, value);
}

template <class T>
constexpr auto Make(unsigned d0, unsigned d1, const T& value) {
  return DVector<T, 2>(d0, Make<T>(d1, value));
}

template <class T>
constexpr auto Make(unsigned d0, unsigned d1, unsigned d2, const T& value) {
  return DVector<T, 3>(d0, Make<T>(d1, d2, value));
}

template <class T>
constexpr auto Make(unsigned d0, unsigned d1, unsigned d2, unsigned d3,
                    const T& value) {
  return DVector<T, 4>(d0, Make<T>(d1, d2, d3, value));
}

template <class T>
constexpr auto Make(unsigned d0, unsigned d1, unsigned d2, unsigned d3,
                    unsigned d4, const T& value) {
  return DVector<T, 5>(d0, Make<T>(d1, d2, d3, d4, value));
}
}  // namespace nvector
