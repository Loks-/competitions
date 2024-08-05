#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix_static_size.h"

#include <utility>

namespace modular {
namespace mstatic {
template <class TModular>
class FibonacciMatrix {
 public:
  using TMatrix = SquareMatrixStaticSize<TModular, 2>;

  // Return { F_{n-1}, F_n }.
  constexpr static std::pair<TModular, TModular> GetPU(uint64_t n) {
    TMatrix fn = (TMatrix{0, 1, 1, 1}).PowU(n);
    return {fn(0, 0), fn(0, 1)};
  }

  constexpr static std::pair<TModular, TModular> GetPS(int64_t n) {
    if (n >= 0) return GetPU(uint64_t(n));
    auto p = GetPU(uint64_t(-n) + 1);
    return (n & 1) ? std::make_pair(-p.second, p.first)
                   : std::make_pair(p.second, -p.first);
  }

  constexpr static TModular GetU(uint64_t n) { return GetPU(n).second; }

  constexpr static TModular GetU(uint64_t n, TModular f0, TModular f1) {
    auto p = GetPU(n);
    return p.first * f0 + p.second * f1;
  }

  constexpr static TModular GetS(int64_t n) { return GetPS(n).second; }

  constexpr static TModular GetS(int64_t n, TModular f0, TModular f1) {
    auto p = GetPS(n);
    return p.first * f0 + p.second * f1;
  }
};
}  // namespace mstatic
}  // namespace modular
