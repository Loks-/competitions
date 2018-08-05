#pragma once

#include "base.h"

class DisjointSet
{
protected:
    uint32_t n;
    vector<uint32_t> p;
    vector<uint32_t> rank;
    vector<uint32_t> vsize;
    stack<uint32_t> ts;
    uint32_t sets;

public:
    DisjointSet(uint32_t n = 0)
    {
        Init(n);
    }

    uint32_t Size() const { return n; }
    uint32_t GetSetsCount() const { return sets; }

    void Init(uint32_t n_)
    {
        n = n_;
        p.resize(n);
        for (uint32_t i = 0; i < n; ++i) {
            p[i] = i;
        }
        rank.resize(n);
        fill(rank.begin(), rank.end(), 0);
        vsize.resize(n);
        fill(vsize.begin(), vsize.end(), 1);
        sets = n;
    }

    void Union(uint32_t i1, uint32_t i2)
    {
        UnionI(Find(i1), Find(i2));
    }

    uint32_t Find(uint32_t x)
    {
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

    uint32_t GetSize(uint32_t x)
    {
        return vsize[Find(x)];
    }

protected:
    void UnionI(uint32_t i1, uint32_t i2)
    {
        if (i1 == i2)
            return;
        --sets;
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
