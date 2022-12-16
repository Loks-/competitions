#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/numeric/bits/first_bit.h"
#include "common/stl/base.h"
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
  auto vs = nvector::ReadLines();
  for (auto s : vs) {
    auto ss = Split(s, " =;,");
    Valve v;
    v.index = vv.size();
    v.name = ss[1];
    v.flow = stoi(ss[5]);
    for (unsigned i = 10; i < ss.size(); ++i) v.dir.push_back(ss[i]);
    vv.push_back(v);
  }
  sort(vv.begin(), vv.end(), [](auto& l, auto& r) { return l.flow > r.flow; });
  unordered_map<string, unsigned> vm;
  unsigned nbits = 0;
  for (unsigned i = 0; i < vv.size(); ++i) {
    vm[vv[i].name] = i;
    if (vv[i].flow) ++nbits;
  }
  DirectedGraph g(vv.size());
  for (unsigned i = 0; i < vv.size(); ++i) {
    for (auto s : vv[i].dir) g.AddEdge(i, vm[s]);
  }
  vector<vector<unsigned>> vvd;
  for (unsigned i = 0; i < g.Size(); ++i)
    vvd.push_back(DistanceFromSource(g, i));

  unordered_map<uint64_t, unsigned> cache;
  std::function<unsigned(unsigned, unsigned, uint64_t)> Solve =
      [&](unsigned time, unsigned index, uint64_t mask) {
        if (mask == 0) return 0u;
        auto key = (time << 16) + index + (mask << 32);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        unsigned best = 0;
        for (uint64_t tmask = mask; tmask;) {
          auto i = numeric::FirstBit(tmask) - 1;
          auto b = (1ull << i);
          tmask &= ~b;
          if (vvd[index][i] + 1 >= time) continue;
          auto time2 = time - vvd[index][i] - 1;
          unsigned current = vv[i].flow * time2 + Solve(time2, i, mask & ~b);
          best = max(best, current);
        }
        cache[key] = best;
        return best;
      };

  auto si = vm["AA"];
  uint64_t mask = (1ull << nbits) - 1ull;
  cout << Solve(30, si, (1ull << nbits) - 1ull) << endl;
  unsigned r2 = 0;
  for (uint64_t p1 = 0; p1 < (1ull << (nbits - 1)); ++p1) {
    auto current = Solve(26, si, p1) + Solve(26, si, mask & ~p1);
    r2 = max(r2, current);
  }
  cout << r2 << endl;
  return 0;
}
