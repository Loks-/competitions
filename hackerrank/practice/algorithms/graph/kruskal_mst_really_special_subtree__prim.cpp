// https://www.hackerrank.com/challenges/kruskalmstrsub

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/graph/graph_ei/mst/prim.h"
#include "common/stl/base.h"

int main_kruskal_mst_really_special_subtree__prim() {
  unsigned n, m;
  cin >> n >> m;
  UndirectedGraphEI<unsigned> g(n);
  g.ReadEdges(m);
  cout << graph::mst::Prim(g, graph::EdgeCostProxy<unsigned>(), -1u, 0).second
       << endl;
  return 0;
}
