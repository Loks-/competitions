#include "common/binary_search_tree/splay_tree.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree = bst::SplayTree<true, TEmpty>;
using TNode = TTree::TNode;

int main_immunization_operation() {
  unsigned T, V, M;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cout << "Case #" << it << ":";
    cin >> V >> M;
    TTree tree(V);
    TNode *root = nullptr, *p1 = nullptr, *p2 = nullptr, *p3 = nullptr,
          *pt = nullptr;
    auto vv = nvector::Read<int64_t>(V);
    auto vp = nvector::Read<int64_t>(V);
    vector<pair<int64_t, int64_t>> vvp;
    for (unsigned i = 0; i < V; ++i) vvp.push_back({vv[i], vp[i]});
    sort(vvp.begin(), vvp.end());
    auto vm = nvector::Read<int64_t>(M);
    unsigned p = 0;
    int64_t l0 = 0;
    for (auto m : vm) {
      unsigned d = 0;
      auto l1 = l0 + m;
      if (root) {
        tree.SplitByKey(root, min(l0, l1), p1, pt);
        tree.SplitByKey(pt, max(l0, l1) + 1, p2, p3);
        if (p2) {
          d += p2->info.size;
          tree.ReleaseTree(p2);
        }
        root = tree.Join(p1, p3);
      }
      if (l1 > l0) {
        for (; (p < V) && (vvp[p].first <= l1); ++p) {
          if ((vvp[p].second >= vvp[p].first) && (vvp[p].second <= l1)) {
            ++d;
          } else {
            root = tree.InsertNewNode(root, {}, vvp[p].second);
          }
        }
      }
      l0 = l1;
      cout << " " << d;
    }
    cout << endl;
    tree.ReleaseTree(root);
  }
  return 0;
}
