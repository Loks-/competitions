#pragma once

#include "../fraction.h"
#include <algorithm>
#include <vector>

class ContinuedFraction
{
public:
    using TCFVector = std::vector<int64_t>;

protected:
    std::vector<int64_t> cf;

public:
    ContinuedFraction(const std::vector<int64_t>& continued_fraction) : cf(continued_fraction)
    {
        if (cf.size() == 0)
            cf.push_back(0);
    }

    const std::vector<int64_t>& GetVector() const { return cf; }
    unsigned Size() const { return unsigned(cf.size()); }
    int64_t operator()(unsigned index) const { return cf[index]; }

    bool operator==(const ContinuedFraction& r) const { return cf == r.cf; }
    bool operator<(const ContinuedFraction& r) const
    {
        unsigned ls = Size(), rs = r.Size(), s = std::min(ls, rs);
        for (unsigned i = 0; i < s; ++i)
        {
            int64_t d = cf[i] - r.cf[i];
            if (d) return ((i & 1) == 0) == (d < 0);
        }
        if (ls == rs) return false;
        return ((s & 1) == 0) != (ls == s);
    }
};
