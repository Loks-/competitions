#pragma once

#include <queue>
#include <vector>

// Return distance to each vertex from required vertex.
// If some vertex is unreachable it returns -1 for it.
template<class TGraph>
inline std::vector<unsigned> GetDistancesFromSource(const TGraph& g, unsigned source)
{
    const unsigned none = -1;
    std::vector<unsigned> d(g.Size(), none);
    d[source] = 0;
    std::queue<unsigned> q;
    q.push(source);
    while (!q.empty())
    {
        unsigned f = q.front(); q.pop();
        unsigned l = d[f];
        for (unsigned k : g.Edges(f))
        {
            if (d[k] == none)
            {
                d[k] = l + 1;
                q.push(k);
            }
        }
    }
    return d;
}
