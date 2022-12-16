#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/numeric/bits/bits_count.h"
#include "common/stl/base.h"
#include "common/stl/hash/pair.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <functional>
#include <unordered_map>

int main_2216() {
  class Valve {
   public:
    unsigned index;
    string name;
    vector<string> dir;
    int64_t flow;
  };

  vector<Valve> vv;
  unordered_map<string, unsigned> vm;
  uint64_t bmask = 0;
  auto vs = nvector::ReadLines();
  for (auto s : vs) {
    auto ss = Split(s, " =;,");
    Valve v;
    v.index = vv.size();
    v.name = ss[1];
    v.flow = stoi(ss[5]);
    for (unsigned i = 10; i < ss.size(); ++i) v.dir.push_back(ss[i]);
    vm[v.name] = v.index;
    vv.push_back(v);
    if (v.flow) bmask |= (1ull << v.index);
  }
  unsigned nbits = numeric::BitsCount(bmask);
  DirectedGraph g(vv.size());
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (auto s : vv[i].dir) g.AddEdge(i, vm[s]);
  }
  vector<vector<unsigned>> vvd;
  for (unsigned i = 0; i < g.Size(); ++i)
    vvd.push_back(DistanceFromSource(g, i));

  using Key = pair<unsigned, uint64_t>;
  unordered_map<Key, unsigned> cache;
  std::function<unsigned(unsigned, unsigned, uint64_t)> Solve =
      [&](unsigned time, unsigned index, uint64_t mask) {
        Key key{(time << 16) + index, mask};
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        unsigned best = 0;
        for (unsigned i = 0; i < vv.size(); ++i) {
          if (vvd[index][i] + 1 >= time) continue;
          auto b = (1ull << i);
          if (b & mask) {
            auto time2 = time - vvd[index][i] - 1;
            unsigned current = vv[i].flow * time2 + Solve(time2, i, mask & ~b);
            best = max(best, current);
          }
        }
        cache[key] = best;
        return best;
      };

  cout << Solve(30, vm["AA"], bmask) << endl;
  unsigned r2 = 0;
  for (uint64_t p1 = 0; p1 < (1ull << nbits) / 2; ++p1) {
    uint64_t bm0 = 0, bm1 = 0;
    for (unsigned i = 0, j = 0; i < vv.size(); ++i) {
      auto b = (1ull << i);
      if (b & bmask) {
        (((1ull << (j++)) & p1) ? bm0 : bm1) |= b;
      }
    }
    auto current = Solve(26, vm["AA"], bm0) + Solve(26, vm["AA"], bm1);
    r2 = max(r2, current);
  }
  cout << r2 << endl;
  return 0;
}
