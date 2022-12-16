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
    unsigned flow;
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
  unsigned flow_total = 0;
  for (unsigned i = 0; i < vv.size(); ++i) {
    vm[vv[i].name] = i;
    if (vv[i].flow) ++nbits;
    flow_total += vv[i].flow;
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
      [&](unsigned closed_flow, unsigned index, uint64_t mask) {
        if (mask == 0) return 0u;
        auto key = index + (mask << 32);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        unsigned best = 1u << 30;
        for (uint64_t tmask = mask; tmask;) {
          auto i = numeric::FirstBit(tmask) - 1;
          auto b = (1ull << i);
          tmask &= ~b;
          auto dt = vvd[index][i] + 1;
          unsigned current =
              closed_flow * dt + Solve(closed_flow - vv[i].flow, i, mask & ~b);
          best = min(best, current);
        }
        cache[key] = best;
        return best;
      };

  auto si = vm["AA"];
  vector<unsigned> vflow(1ull << nbits, 0), vs2(1ull << nbits, 0);
  uint64_t mask = vflow.size() - 1;
  unsigned r1 = 0, r2 = 0;
  for (unsigned j = 1; j < vflow.size(); ++j) {
    auto i = numeric::FirstBit(j) - 1;
    vflow[j] = vflow[j ^ (1ull << i)] + vv[i].flow;
    auto s = Solve(vflow[j], si, j);
    r1 = max(r1, 30 * vflow[j] - s);
    vs2[j] = 26 * vflow[j] - min(s, 26 * vflow[j]);
  }
  cout << r1 << endl;
  auto vs3 = vs2;
  for (unsigned j = 0; j < vflow.size(); ++j) {
    for (uint64_t tj = j; tj;) {
      auto i = numeric::FirstBit(tj) - 1;
      auto b = (1ull << i);
      tj &= ~b;
      vs3[j] = max(vs3[j], vs3[j ^ b]);
    }
    r2 = max(r2, vs3[j] + vs2[mask ^ j]);
  }
  cout << r2 << endl;
  return 0;
}
