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
    Iterator(uint64_t _maxn) : maxn(_maxn), p(nullptr) {}
    Iterator(PrimesGenerator* _p) : maxn(0), p(_p) {}

    uint64_t operator*() const { return p->Get(); }
    void operator++() { p->Next(); }
    bool operator!=(const Iterator& e) const { return p->Get() <= e.maxn; }
  };

 protected:
  uint64_t maxn;
  PrimesGenerator pg;

 public:
  PrimesRange(uint64_t _maxn) : maxn(_maxn) {}
  Iterator begin() { return Iterator(&pg); }
  Iterator end() { return Iterator(maxn); }
};
}  // namespace factorization
