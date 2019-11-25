#pragma once

#include "common/modular/static/factorial.h"

template <class TModular>
class ModularCatalan : public modular::mstatic::Factorial<TModular, true> {
 public:
  using TBase = modular::mstatic::Factorial<TModular, true>;

  TModular Catalan(unsigned n) {
    TBase::Adjust(2 * n + 1);
    return TBase::vf[2 * n] * TBase::vfi[n] * TBase::vfi[n + 1];
  }
};
