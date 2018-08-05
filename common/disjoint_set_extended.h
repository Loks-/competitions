#pragma once

#include "base.h"

template <typename TTData>
class DisjointSetExtended
{
public:
    using TData = TTData;

protected:
    unordered_map<TData, uint32_t> m;
    vector<uint32_t> p;
    vector<uint32_t> rank;
    vector<uint32_t> vsize;
    stack<uint32_t> ts;
    uint32_t unions = 0;

public:
    uint32_t Size() const { return p.size(); }
    uint32_t GetUnions() const { return unions; }

    uint32_t GetIndex(const TData& v)
    {
        auto it = m.find(v);
        if (it != m.end())
            return it->second;
        uint32_t index = Size();
        m[v] = index;
        p.push_back(index);
        rank.push_back(0);
        vsize.push_back(1);
        return index;
    }
    
    uint32_t Find(const TData& v)
    {
        uint32_t x = GetIndex(v);
        uint32_t px = p[x];
        if (px == x)
            return px;
        uint32_t ppx = p[px];
        if (ppx == px)
            return px;
        do
        {
            ts.push(x);
            x = px;
            px = ppx;
            ppx = p[px];
        } while (px != ppx);
        while (!ts.empty())
        {
            x = ts.top();
            p[x] = ppx;
            ts.pop();
        }
        return ppx;
    }

    uint32_t GetSize(const TData& v)
    {
        return vsize[Find(v)];
    }

    void Union(const TData& v1, const TData& v2)
    {
        UnionI(Find(v1), Find(v2));
    }

protected:
    void UnionI(uint32_t i1, uint32_t i2)
    {
        if (i1 == i2)
            return;
        ++unions;
        if (rank[i1] > rank[i2])
        {
            p[i2] = i1;
            vsize[i1] += vsize[i2];
        }
        else
        {
            p[i1] = i2;
            if (rank[i1] == rank[i2])
                ++rank[i1];
            vsize[i2] += vsize[i1];
        }
    }
};
