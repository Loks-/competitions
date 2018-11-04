#pragma once

#include "../fraction.h"
#include <vector>

class ContinuedFraction
{
protected:
    std::vector<int64_t> cf;

public:
    ContinuedFraction(const std::vector<int64_t>& continued_fraction) : cf(continued_fraction)
    {
        if (cf.size() == 0)
            cf.push_back(0);
    }

    unsigned Size() const { return unsigned(cf.size()); }
    int64_t operator()(unsigned index) const { return cf[index]; }
};
