// https://www.hackerrank.com/challenges/heavy-light-white-falcon

#include "common/binary_search_tree/info/max.h"
#include "common/binary_search_tree/info/none.h"
#include "common/graph/tree.h"
#include "common/graph/tree/link_cut_tree.h"
#include "common/stl/base.h"

int main_heavy_light_white_falcon__lct() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  graph::LinkCutTree<unsigned, bst::info::Max<unsigned, bst::info::None>> lct(
      tree);

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, x;
    cin >> t >> u >> x;
    if (t == 1)
      lct.SetData(u, x);
    else if (t == 2)
      cout << lct.GetPathInfo(u, x).max << endl;
  }
  return 0;
}
