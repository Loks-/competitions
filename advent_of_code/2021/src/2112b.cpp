#include "common/graph/graph.h"
#include "common/graph/graph/total_paths_unique_vertices.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

int main_2112b() {
  auto vs = nvector::ReadLines();
  vector<string> vl;
  for (auto s : vs) {
    auto ss = Split(s, '-');
    vl.push_back(ss[0]);
    vl.push_back(ss[1]);
  }
  nvector::UniqueUnsorted(vl);
  unordered_map<string, unsigned> um;
  for (unsigned i = 0; i < vl.size(); ++i) um[vl[i]] = i;
  auto fs = lower_bound(vl.begin(), vl.end(), "a") - vl.begin();
  UndirectedGraph g0(vl.size());
  for (auto s : vs) {
    auto ss = Split(s, '-');
    g0.AddEdge(um[ss[0]], um[ss[1]]);
  }
  UndirectedGraph g(vl.size() - fs + 1);
  for (unsigned i = 0; i < fs; ++i) {
    for (auto u1 : g0.Edges(i)) {
      g.AddBaseEdge(u1 - fs, u1 - fs);
      for (auto u2 : g0.Edges(i)) {
        if (u1 < u2) g.AddEdge(u1 - fs, u2 - fs);
      }
    }
  }
  for (unsigned i = fs; i < vl.size(); ++i) {
    for (auto u : g0.Edges(i)) {
      if (i < u) g.AddEdge(i - fs, u - fs);
    }
  }
  unsigned bb = um["start"] - fs, e = um["end"] - fs;
  uint64_t r0 = TotalPathsUniqueVertices(g, bb, e), rs = r0;
  for (unsigned b = 0; b < g.Size() - 1; ++b) {
    if ((b == bb) || (b == e)) continue;
    UndirectedGraph g1(g);
    for (auto u : g.Edges(b)) g1.AddEdge(g.Size() - 1, u);
    auto r = TotalPathsUniqueVertices(g1, bb, e),
         rd = TotalPathsUniqueVertices(g, bb, e,
                                       ((1ull << g.Size()) - 1) ^ (1ull << b));
    rs += (r + rd) / 2 - r0;
  }
  cout << rs << endl;
  return 0;
}
