#pragma once

#include "../arithmetic.h"
#include "../utils/primitive_root.h"
#include "../../base.h"
#include "../../factorization/base.h"
#include "../../factorization/utils/factorize.h"
#include <vector>

// Calculate discrete logarithm.
// Prebuild:
//   O(P) time, O(P) memory
// Log:
//   O(1) time
class PModularDiscreteLogFullMap {
 public:
  using TModularA = TModularArithmetic_P32U;

 protected:
  uint64_t p, primitive;
  std::vector<unsigned> vmap;

 public:
  void Build() {
    vmap.resize(p);
    uint64_t r = 1;
    for (uint64_t i = 0; i + 1 < p; ++i) {
      vmap[r] = i;
      r = TModularA::Mult(r, primitive, p);
    }
  }

  PModularDiscreteLogFullMap(unsigned prime, unsigned pprimitive)
      : p(prime), primitive(pprimitive) {
    Build();
  }

  PModularDiscreteLogFullMap(unsigned prime,
                             const TFactorization& p1_factorization)
      : PModularDiscreteLogFullMap(
            prime, FindSmallestPrimitive(prime, p1_factorization)) {}
  PModularDiscreteLogFullMap(unsigned prime)
      : PModularDiscreteLogFullMap(prime, Factorize(prime - 1)) {}

  unsigned Log(uint64_t x) const {
    x = TModularA::ApplyU(x, p);
    assert(x);
    return vmap[x];
  }
};
