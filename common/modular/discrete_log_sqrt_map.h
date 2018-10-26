#pragma once

#include "modular_arithmetic_proxy.h"
#include "utils/primitive_root.h"
#include "../base.h"
#include "../factorization/base.h"
#include "../numeric/utils/usqrt.h"
#include <algorithm>
#include <unordered_map>

// Calculate discrete logarithm using "Baby-step giant-step" algorithm ( https://en.wikipedia.org/wiki/Baby-step_giant-step ).
// Default m = sqrt(P).
// Prebuild:
//   O(m) time, O(m) memory
// Log:
//   O(P / m) time
template<class TModularProxy = TModularArithmeticProxy_P32U>
class DiscreteLogSqrtMap
{
protected:
    TModularProxy proxy;
    uint64_t primitive;
    uint64_t m, ippm; 
    std::unordered_map<uint64_t, uint64_t> vmap;

public:
    void Build()
    {
        uint64_t p = proxy.GetMod();
        vmap.clear();
        uint64_t r = 1;
        for (uint64_t i = 0; i < m; ++i)
        {
            vmap[r] = i;
            r = proxy.Mult(r, primitive);
        }
        ippm = proxy.Inverse(proxy.PowU(primitive, m));
    }

    DiscreteLogSqrtMap(uint64_t p, uint64_t pprimitive) : proxy(p), primitive(pprimitive) { m = USqrt(p) + 1; Build(); }
    DiscreteLogSqrtMap(uint64_t p, uint64_t pprimitive, uint64_t size) : proxy(p), primitive(pprimitive) { m = std::min(size, p-1); Build(); }
    DiscreteLogSqrtMap(uint64_t p, const TFactorization& p1_factorization) : DiscreteLogSqrtMap(p, FindSmallestPrimitive(p, p1_factorization)) {}
    DiscreteLogSqrtMap(uint64_t p, const TFactorization& p1_factorization, uint64_t size) : DiscreteLogSqrtMap(p, FindSmallestPrimitive(p, p1_factorization), size) {}

    uint64_t Log(uint64_t x) const
    {
        x = proxy.ApplyU(x);
        assert(x);
        for (uint64_t i = 0; i * m <= proxy.GetMod(); ++i)
        {
            auto it = vmap.find(x);
            if (it != vmap.end())
                return i * m + it->second;
            x = proxy.Mult(x, ippm);
        }
        assert(false);
        return 0;
    }
};
