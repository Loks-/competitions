#pragma once

#include "common/modular/static/factorial.h"

namespace modular {
namespace mstatic {
template <class TModular>
class Catalan : public Factorial<TModular, true> {
 public:
  using TBase = Factorial<TModular, true>;

  constexpr TModular Get(unsigned n) {
    TBase::Adjust(2 * n + 1);
    return TBase::vf[2 * n] * TBase::vfi[n] * TBase::vfi[n + 1];
  }

  constexpr TModular GetTriangle(unsigned n, unsigned m) {
    TBase::Adjust(n + m + 1);
    return TBase::vf[n + m] * TModular(n - m + 1) * TBase::vfi[n + 1] *
           TBase::vfi[m];
  }

  constexpr TModular GetConvolution(unsigned n, unsigned k) {
    TBase::Adjust(2 * n + k + 1);
    return TBase::vf[2 * n + k] * TModular(k + 1) * TBase::vfi[n] *
           TBase::vfi[n + k + 1];
  }
};
}  // namespace mstatic
}  // namespace modular
