// https://www.hackerrank.com/challenges/kruskalmstrsub

#include "common/graph/graph_ei.h"
#include "common/graph/graph_ei/minimum_spanning_tree.h"
#include "common/stl/base.h"

int main_kruskal_mst_really_special_subtree() {
  unsigned n, m;
  cin >> n >> m;
  UndirectedGraphEI<unsigned> g(n);
  g.ReadEdges(m);
  cout << MinimumSpanningTree(g, -1u).second << endl;
  return 0;
}
