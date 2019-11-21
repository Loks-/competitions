// https://www.hackerrank.com/challenges/james-tree

#include "common/graph/tree/lca.h"
#include "common/graph/tree_ei.h"
#include "common/graph/tree_ei/nodes_info.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<1000000009>;
using TFactorial = ModularFactorial<TModular>;
using TTree = TreeGraphEI<TModular>;

// We don't need LCA here, but with LCA code is simpler.
int main_magic_number_tree() {
  TFactorial f;
  unsigned n;
  cin >> n;
  TTree tree(n);
  tree.ReadEdges();
  graph::LCA lca(tree);
  graph::TreeEINodesInfo<TModular> nodes_info(tree);
  TModular r = 0;
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = i + 1; j < n; ++j) {
      unsigned a = lca.GetLCA(i, j);
      unsigned d =
          nodes_info.deep[i] + nodes_info.deep[j] - 2 * nodes_info.deep[a];
      TModular l = nodes_info.distance_to_root[i] +
                   nodes_info.distance_to_root[j] -
                   nodes_info.distance_to_root[a] * 2;
      r += l * 2 * f.GetI(d + 1) * f.Get(d);
    }
  }
  cout << r * f.Get(n) << endl;
  return 0;
}
