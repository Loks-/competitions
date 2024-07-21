#include "common/factorization/factorization.h"
#include "common/graph/graph_ei/shortest_cycle/undirected_vertex_exclusion.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/unique.h"

#include <unordered_map>

using TGraph = UndirectedGraphEI<unsigned>;

int main_1325e() {
  unsigned N;
  cin >> N;
  auto v = nvector::Read<unsigned>(N);
  vector<vector<unsigned>> vvp;
  vector<unsigned> vp(1, 1u);
  for (auto u : v) {
    vector<unsigned> uvp;
    for (auto& p : Factorize(u)) {
      if (p.power & 1) {
        uvp.push_back(p.prime);
        vp.push_back(p.prime);
      }
    }
    for (; uvp.size() < 2;) uvp.push_back(1);
    vvp.push_back(uvp);
  }
  nvector::UniqueUnsorted(vp);
  TGraph g(vp.size());
  for (auto& v : vvp)
    g.AddEdge(lower_bound(vp.begin(), vp.end(), v[0]) - vp.begin(),
              lower_bound(vp.begin(), vp.end(), v[1]) - vp.begin(), 1);
  auto answer = graph::scycle::VertexExclusion(g, N + 1);
  std::cout << ((answer <= N) ? int(answer) : -1) << std::endl;
  return 0;
}
