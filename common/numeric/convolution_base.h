#pragma once

#include <vector>

namespace numeric {
template <class T>
constexpr std::vector<T> ConvolutionBase(const std::vector<T>& a,
                                         const std::vector<T>& b) {
  if (a.empty() || b.empty()) return {};
  std::vector<T> r(a.size() + b.size() - 1);
  for (unsigned i = 0; i < a.size(); ++i) {
    auto ai = a[i];
    for (unsigned j = 0; j < b.size(); ++j) r[i + j] += ai * b[j];
  }
  return r;
}

template <class T>
constexpr std::vector<T> ConvolutionBase(const std::vector<T>& a) {
  return ConvolutionBase(a, a);
}
}  // namespace numeric
