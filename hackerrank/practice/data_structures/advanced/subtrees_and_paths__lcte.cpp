// https://www.hackerrank.com/challenges/subtrees-and-paths

#include "common/graph/tree.h"
#include "common/graph/tree/lcte/action/add_each__max.h"
#include "common/graph/tree/lcte/info/max.h"
#include "common/graph/tree/lcte/info/none.h"
#include "common/graph/tree/lcte/lcte.h"
#include "common/stl/base.h"

#include <string>

using namespace graph::lcte;

int main_subtrees_and_paths__lcte() {
  unsigned N, Q;
  cin >> N;
  TreeGraph tree(N);
  tree.ReadEdges();
  LCTE<int64_t, info::PMax<int64_t>, info::VMax<int64_t>,
       action::AddEachMax<int64_t>>
      lcte(tree);

  cin >> Q;
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    string s;
    int u, x;
    cin >> s >> u >> x;
    if (s == "add")
      lcte.AddActionToSubtree(tree.GetRoot(), u - 1, x);
    else if (s == "max")
      cout << lcte.GetPathInfo(u - 1, x - 1).pmax << endl;
  }
  return 0;
}
