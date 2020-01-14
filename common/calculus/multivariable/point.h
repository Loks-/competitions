#pragma once

#include <array>

namespace calculus {
namespace multivariable {
template <class TValue, unsigned _dim>
class Point {
 public:
  static const unsigned dim = _dim;
  std::array<TValue, dim> x;

  const TValue& operator()(unsigned i) { return x[i]; }
};
}  // namespace multivariable
}  // namespace calculus
