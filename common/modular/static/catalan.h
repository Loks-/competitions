#pragma once

#include "common/modular/static/factorial.h"

namespace modular {
namespace mstatic {
template <class TModular>
class Catalan : public Factorial<TModular, true> {
 public:
  using TBase = Factorial<TModular, true>;

  TModular Get(unsigned n) {
    TBase::Adjust(2 * n + 1);
    return TBase::vf[2 * n] * TBase::vfi[n] * TBase::vfi[n + 1];
  }
};
}  // namespace mstatic
}  // namespace modular
