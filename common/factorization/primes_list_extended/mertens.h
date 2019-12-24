#pragma once

#include "common/base.h"
#include "common/factorization/primes_list_extended/mobius.h"
#include <vector>

namespace factorization {
namespace ple {
class Mertens : public Mobius {
 public:
  using TBase = Mobius;

 protected:
  std::vector<int> mertens;

  void Precalc() {
    assert(TBase::mobius.size() == TBase::table_size + 1);
    mertens.resize(TBase::table_size + 1);
    mertens[0] = 0;
    for (unsigned i = 1; i <= TBase::table_size; ++i)
      mertens[i] = mertens[i - 1] + TBase::mobius[i];
  }

 public:
  Mertens(uint64_t size) : TBase(size) { Precalc(); }

  int GetMobius(uint64_t n) const { return TBase::Get(n); }

  int Get(uint64_t n) const {
    assert(n < mertens.size());
    return mertens[n];
  }

  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace ple
}  // namespace factorization
