#pragma once

#include "common/polynomial/base_newton_polynomial.h"
#include "common/modular/modular.h"

#include <iostream>
#include <vector>

class TesterInterpolation
{
public:
    using TModular = Modular<>;
    static const unsigned power1 = 10, power2 = 100, n = 1000, k = 100;

protected:
    std::vector<TModular> vp1, vp2;

public:
    void Init()
    {
        vp1.resize(n);
        vp2.resize(n);
        vp1[0] = vp2[0] = TModular(0);
        for (unsigned i = 1; i < n; ++i)
        {
            vp1[i] = vp1[i-1] + TModular(i).PowU(power1);
            vp2[i] = vp2[i-1] + TModular(i).PowU(power2);
        }
    }

    TesterInterpolation() { Init(); }

    static bool TestBaseNewtonPolynomial(unsigned power, const std::vector<TModular>& vp)
    {
        std::vector<TModular> vtemp(vp.begin(), vp.begin() + power + 2);
        BaseNewtonPolynomial<TModular> p;
        p.Interpolate(vtemp);
        for (unsigned i = n - k; i < n; ++i)
        {
            if (p(TModular(i)) != vp[i])
            {
                std::cout << "TestBaseNewtonPolynomial failed:\n\tpower = " << power << "\tindex = " << i << std::endl;
                return false;
            }
        }
        return true;
    }

    static bool TestAll(unsigned power, const std::vector<TModular>& vp)
    {
        bool b = true;
        b = TestBaseNewtonPolynomial(power, vp) && b;
        return b;
    }

    bool TestAll() const
    {
        return TestAll(power1, vp1) && TestAll(power2, vp2);
    }
};
