#pragma once

#include "common/modular/static/factorial.h"

template <class TModular>
class ModularCatalan : public ModularFactorial<TModular, true> {
 public:
  using TBase = ModularFactorial<TModular, true>;

  TModular Catalan(unsigned n) {
    TBase::Adjust(2 * n + 1);
    return TBase::vf[2 * n] * TBase::vfi[n] * TBase::vfi[n + 1];
  }
};
