// https://www.hackerrank.com/challenges/kruskalmstrsub

#include "common/graph/graph_ei/mst/kruskal.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/stl/base.h"

int main_kruskal_mst_really_special_subtree__kruskal() {
  unsigned n, m;
  cin >> n >> m;
  UndirectedGraphEI<unsigned> g(n);
  g.ReadEdges(m);
  cout << graph::mst::Kruskal(g, graph::EdgeCostProxy<unsigned>()).second
       << endl;
  return 0;
}
