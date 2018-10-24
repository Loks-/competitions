#pragma once

#include "modular_arithmetic_proxy.h"
#include "utils/primitive_root.h"
#include "../base.h"
#include "../factorization/base.h"
#include "../factorization/utils/factorize.h"
#include <vector>

// Calculate discrete logarithm.
// Prebuild:
//   O(P) time, O(P) memory
// Log:
//   O(1) time
class DiscreteLogFullMap
{
public:
    using TModularProxy = TModularArithmeticProxy_P32U;

protected:
    TModularProxy proxy;
    uint64_t primitive;
    std::vector<unsigned> vmap;

public:
    void Build()
    {
        unsigned p = unsigned(proxy.GetMod());
        vmap.resize(p);
        uint64_t r = 1;
        for (uint64_t i = 0; i + 1 < p; ++i)
        {
            vmap[r] = i;
            r = proxy.Mult(r, primitive);
        }
    }

    DiscreteLogFullMap(unsigned p, unsigned pprimitive) : proxy(p), primitive(pprimitive) { Build(); }
    DiscreteLogFullMap(unsigned p, const TFactorization& p1_factorization) : DiscreteLogFullMap(p, FindSmallestPrimitive(p, p1_factorization)) {}
    DiscreteLogFullMap(unsigned p) : DiscreteLogFullMap(p, Factorize(p - 1)) {}

    unsigned Log(uint64_t x) const
    {
        x = proxy.ApplyU(x);
        assert(x);
        return vmap[x];
    }
};
