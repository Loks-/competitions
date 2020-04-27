// https://www.hackerrank.com/challenges/subsequence-weighting

#include "common/binary_search_tree/info/max.h"
#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree =
    bst::Treap<true, false, uint64_t, bst::info::Max<uint64_t, bst::info::None>,
               bst::action::None, uint64_t>;
using TNode = TTree::TNode;

int main_subsequence_weighting__treap() {
  unsigned T, N;
  cin >> T;
  TTree tree(150000);
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    vector<uint64_t> va = ReadVector<uint64_t>(N), vw = ReadVector<uint64_t>(N);
    TNode *root = 0, *l, *r;
    for (unsigned i = 0; i < N; ++i) {
      tree.SplitByKey(root, va[i], l, r);
      TNode* m = tree.New(vw[i] + (l ? l->info.max : 0), va[i]);
      root = tree.Join(tree.Join(l, m), r);
    }
    cout << root->info.max << endl;
    tree.ReleaseTree(root);
  }
  return 0;
}
