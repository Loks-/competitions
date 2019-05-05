// https://www.hackerrank.com/challenges/even-tree

#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/stl/base.h"

int main_even_tree() {
  unsigned n, m, s = 0;
  cin >> n >> m;
  TreeGraph t(n);
  t.ReadEdges();
  graph::TreeNodesInfo ni(t);
  for (unsigned i = 0; i < n; ++i) {
    if ((ni.subtree_size[i] % 2) == 0) ++s;
  }
  cout << s - 1 << endl;
  return 0;
}
