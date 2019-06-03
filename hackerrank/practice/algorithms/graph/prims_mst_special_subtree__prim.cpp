// https://www.hackerrank.com/challenges/primsmstsub

#include "common/graph/graph_ei/mst/prim.h"
#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/edge_cost_proxy.h"
#include "common/stl/base.h"

int main_prims_mst_special_subtree__prim() {
  unsigned n, m, k;
  cin >> n >> m;
  UndirectedGraphEI<unsigned> g(n);
  g.ReadEdges(m);
  cin >> k;
  cout << graph::mst::Prim(g, graph::EdgeCostProxy<unsigned>(), -1u, k - 1)
              .second
       << endl;
  return 0;
}
