// https://www.hackerrank.com/challenges/subtrees-and-paths

#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/segment_tree/action/add_each__max.h"
#include "common/segment_tree/info/max.h"
#include "common/stl/base.h"

#include <string>

int main_subtrees_and_paths() {
  unsigned N, Q;
  cin >> N;
  TreeGraph tree(N);
  tree.ReadEdges();
  graph::HLD<int64_t, st::info::Max<int64_t>, st::action::AddEachMax<int64_t>>
      hld(tree);

  cin >> Q;
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    string s;
    int u, x;
    cin >> s >> u >> x;
    if (s == "add")
      hld.Subtree(u - 1).AddAction(x);
    else if (s == "max")
      cout << hld.Path(u - 1, x - 1).GetInfo().max << endl;
  }
  return 0;
}
