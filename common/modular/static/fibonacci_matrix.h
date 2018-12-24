#pragma once

#include "../../base.h"
#include "../../linear_algebra/matrix_static_size.h"
#include <utility>

template <class TModular>
class ModularFibonacciMatrix {
 public:
  using TMatrix = TSquareMatrixStaticSize<TModular, 2>;

  // Return { F_{n-1}, F_n }.
  static std::pair<TModular, TModular> GetPU(uint64_t n) {
    TMatrix transition;
    transition(0, 0) = 0;
    transition(0, 1) = 1;
    transition(1, 0) = 1;
    transition(1, 1) = 1;
    TMatrix fn = transition.PowU(n);
    return {fn(0, 0), fn(0, 1)};
  }

  static std::pair<TModular, TModular> GetPS(int64_t n) {
    if (n >= 0) return GetPU(uint64_t(n));
    auto p = GetPU(uint64_t(-n) + 1);
    return (n & 1) ? std::make_pair(-p.second, p.first)
                   : std::make_pair(p.second, -p.first);
  }

  static TModular GetU(uint64_t n) { return GetPU(n).second; }

  static TModular GetU(uint64_t n, TModular f0, TModular f1) {
    auto p = GetPU(n);
    return p.first * f0 + p.second * f1;
  }

  static TModular GetS(int64_t n) { return GetPS(n).second; }

  static TModular GetS(int64_t n, TModular f0, TModular f1) {
    auto p = GetPS(n);
    return p.first * f0 + p.second * f1;
  }
};
