#pragma once

#include <array>

namespace calculus {
namespace multivariable {
template <class TValue, unsigned dim>
class Point {
 public:
  static consteval unsigned Dim() { return dim; }

 public:
  std::array<TValue, dim> x;

 public:
  constexpr const TValue& operator()(unsigned i) const { return x[i]; }
};
}  // namespace multivariable
}  // namespace calculus
