#pragma once

#include "common/base.h"
#include "common/factorization/primes_generator.h"

namespace factorization {
class PrimesRange {
 public:
  class Iterator {
   protected:
    uint64_t maxn;
    PrimesGenerator* p;

   public:
    constexpr Iterator(uint64_t _maxn) : maxn(_maxn), p(nullptr) {}
    constexpr Iterator(PrimesGenerator* _p) : maxn(0), p(_p) {}

    constexpr uint64_t operator*() const { return p->Get(); }

    constexpr void operator++() { p->Next(); }

    constexpr bool operator!=(const Iterator& e) const {
      return p->Get() <= e.maxn;
    }
  };

 protected:
  uint64_t maxn;
  PrimesGenerator pg;

 public:
  constexpr explicit PrimesRange(uint64_t _maxn) : maxn(_maxn) {}
  constexpr Iterator begin() { return Iterator(&pg); }
  constexpr Iterator end() { return Iterator(maxn); }
};
}  // namespace factorization
