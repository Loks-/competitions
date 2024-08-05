#pragma once

#include "common/base.h"
#include "common/factorization/table/mobius.h"

#include <vector>

namespace factorization {
namespace table {
class Mertens : public Mobius {
 public:
  using TBase = Mobius;

 protected:
  std::vector<int> mertens;

 public:
  constexpr explicit Mertens(uint64_t size) : TBase(size) {
    mertens.resize(TBase::table_size + 1);
    mertens[0] = 0;
    for (uint64_t i = 1; i <= TBase::table_size; ++i)
      mertens[i] = mertens[i - 1] + TBase::mobius[i];
  }

  constexpr int GetMobius(uint64_t n) const { return TBase::Get(n); }

  constexpr int Get(uint64_t n) const { return mertens[n]; }

  constexpr int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
