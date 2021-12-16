#include "common/graph/graph/distance.h"
#include "common/graph/graph_ei.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <queue>
#include <unordered_map>
#include <utility>

int main_2007b() {
  vector<string> vs = nvector::ReadLines();
  unordered_map<string, unsigned> msi;
  unsigned index = 0;
  vector<tuple<unsigned, unsigned, unsigned>> vp;
  for (auto s : vs) {
    auto ss = Split(s, ' ');
    auto key = ss[0] + " " + ss[1];
    if (msi.find(key) == msi.end()) msi[key] = index++;
    auto ukey = msi[key];
    for (unsigned i = 1; 4 * i + 3 < ss.size(); ++i) {
      auto key2 = ss[4 * i + 1] + " " + ss[4 * i + 2];
      if (msi.find(key2) == msi.end()) msi[key2] = index++;
      vp.push_back({ukey, msi[key2], unsigned(stoi(ss[4 * i]))});
    }
  }
  unsigned source = msi["shiny gold"];
  DirectedGraphEI<uint64_t> g(index);
  for (auto p : vp) g.AddEdge(std::get<0>(p), std::get<1>(p), std::get<2>(p));
  // TODO: Use topological sort
  vector<uint64_t> vc(index);
  vc[source] = 1;
  queue<unsigned> q;
  uint64_t r = 0;
  for (q.push(source); !q.empty(); q.pop()) {
    auto u = q.front();
    if (!vc[u]) continue;
    r += vc[u];
    for (auto e : g.EdgesEI(u)) {
      vc[e.to] += vc[u] * e.info;
      q.push(e.to);
    }
    vc[u] = 0;
  }
  cout << r - 1 << endl;
  return 0;
}
