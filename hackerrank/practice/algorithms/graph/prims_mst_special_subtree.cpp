// https://www.hackerrank.com/challenges/primsmstsub

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/minimum_spanning_tree.h"
#include "common/stl/base.h"

int main_prims_mst_special_subtree() {
  unsigned n, m, k;
  cin >> n >> m;
  UndirectedGraphEI<unsigned> g(n);
  g.ReadEdges(m);
  cin >> k;
  cout << MinimumSpanningTree(g, -1u).second << endl;
  return 0;
}
