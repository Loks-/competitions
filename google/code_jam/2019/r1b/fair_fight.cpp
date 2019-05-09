#include "common/binary_search_tree/info/max.h"
#include "common/binary_search_tree/info/min.h"
#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree =
    Treap<true, true, int, BSTInfoMin<int, BSTInfoMax<int, BSTInfoNone>>,
          BSTActionNone, int>;
using TNode = TTree::TNode;

int main_fair_fight() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int n, k;
    cin >> n >> k;
    TTree tree(2 * n + 4);
    vector<vector<int>> vv{ReadVector<int>(n), ReadVector<int>(n)};
    vector<TNode*> vh(4, 0);
    for (int j = 0; j < 2; ++j) {
      for (int i = 0; i < n; ++i) {
        TNode* p = tree.GetNewNode(i, vv[j][i]);
        vh[2 * j + 1] = tree.InsertByKey(vh[2 * j + 1], p);
      }
    }
    for (int j = 0; j < 4; ++j) {
      TNode* p = tree.GetNewNode((j & 1) ? n : -1, 300000);
      vh[j] = tree.InsertByKey(vh[j], p);
    }
    int64_t total = 0;
    TNode *p0, *p1;
    for (int i = 0; i < n; ++i) {
      int k0 = vv[0][i];
      tree.SplitByKey(vh[0], k0, p0, p1);
      int l0 = p1->info.subtree_max;
      vh[0] = tree.Join(p0, p1);
      tree.SplitByKey(vh[1], k0 + 1, p0, p1);
      int r0 = p1->info.subtree_min;
      vh[1] = tree.Join(p0, p1);
      tree.SplitByKey(vh[2], k0 + k + 1, p0, p1);
      int l1 = p1->info.subtree_max;
      vh[2] = tree.Join(p0, p1);
      tree.SplitByKey(vh[3], k0 + k + 1, p0, p1);
      int r1 = p1->info.subtree_min;
      vh[3] = tree.Join(p0, p1);
      tree.SplitByKey(vh[2], k0 - k, p0, p1);
      int l2 = p1->info.subtree_max;
      vh[2] = tree.Join(p0, p1);
      tree.SplitByKey(vh[3], k0 - k, p0, p1);
      int r2 = p1->info.subtree_min;
      vh[3] = tree.Join(p0, p1);
      l1 = max(l0, l1);
      l2 = max(l0, l2);
      r1 = min(r0, r1);
      r2 = min(r0, r2);
      total += int64_t(i - l1) * (r1 - i);
      total -= int64_t(i - l2) * (r2 - i);
      for (unsigned j = 0; j < 2; ++j) {
        TNode* p = tree.GetNodeByRawIndex(i + j * n);
        vh[2 * j + 1] = tree.RemoveByNode(p);
        vh[2 * j] = tree.InsertByKey(vh[2 * j], p);
      }
    }
    cout << "Case #" << it << ": " << total << std::endl;
  }
  return 0;
}
