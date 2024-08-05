#pragma once

#include "common/base.h"
#include "common/factorization/base.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/arithmetic.h"
#include "common/modular/utils/primitive_root.h"

#include <vector>

// Calculate discrete logarithm.
// Prebuild:
//   O(P) time, O(P) memory
// Log:
//   O(1) time
namespace modular {
namespace proxy {
class DiscreteLogFullMap {
 public:
  using TModularA = TArithmetic_P32U;

 protected:
  uint64_t p, primitive;
  std::vector<unsigned> vmap;

 public:
  constexpr void Build() {
    vmap.resize(p);
    uint64_t r = 1;
    for (uint64_t i = 0; i + 1 < p; ++i) {
      vmap[r] = i;
      r = TModularA::Mult(r, primitive, p);
    }
  }

  constexpr DiscreteLogFullMap(unsigned prime, unsigned pprimitive)
      : p(prime), primitive(pprimitive) {
    Build();
  }

  constexpr DiscreteLogFullMap(unsigned prime,
                               const TFactorization& p1_factorization)
      : DiscreteLogFullMap(
            prime, FindSmallestPrimitiveRoot(prime, p1_factorization)) {}

  constexpr explicit DiscreteLogFullMap(unsigned prime)
      : DiscreteLogFullMap(prime, FactorizeBase(prime - 1)) {}

  constexpr unsigned Log(uint64_t x) const {
    x = TModularA::ApplyU(x, p);
    assert(x);
    return vmap[x];
  }
};
}  // namespace proxy
}  // namespace modular
