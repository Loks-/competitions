// https://www.hackerrank.com/challenges/array-and-simple-queries

#include "common/binary_search_tree/base/traversal.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/utils/swap_intervals.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

using TSplayTree = bst::SplayTree<false, int>;
using TNode = TSplayTree::TNode;

int main_array_and_simple_queries__splay() {
  unsigned N, M;
  cin >> N >> M;
  vector<int> va = ReadVector<int>(N);
  TSplayTree splay_tree(N);
  TNode* root = splay_tree.Build(va);
  for (; M; --M) {
    unsigned t, i, j;
    cin >> t >> i >> j;
    if (t == 1)
      bst::SwapIntervals<TSplayTree>(root, 0, i - 1, j);
    else
      bst::SwapIntervals<TSplayTree>(root, i - 1, j, N);
  }
  vector<int> vf =
      bst::Traverse<TNode, int>(root, bst::ETraversalOrder::Inorder);
  cout << abs(vf[0] - vf.back()) << endl;
  WriteVector(vf);
  return 0;
}
