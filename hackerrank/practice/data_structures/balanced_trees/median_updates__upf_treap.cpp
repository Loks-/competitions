// https://www.hackerrank.com/challenges/median

#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/median.h"
#include "common/stl/base.h"

#include <iomanip>

using TTree = bst::Treap<true, false, TEmpty>;
using TNode = TTree::TNode;

int main_median_updates__upf_treap() {
  cout << setprecision(1) << fixed;
  unsigned N;
  cin >> N;
  TTree tree(N);
  TNode* root = 0;
  for (unsigned i = 0; i < N; ++i) {
    char c;
    int64_t x;
    cin >> c >> x;
    x *= 2;
    if (c == 'a') {
      root = tree.InsertNewNode(root, TEmpty(), x);
    } else if (c == 'r') {
      TNode* node = TTree::FindByKey(root, x);
      if (node == 0) {
        cout << "Wrong!" << endl;
        continue;
      }
      root = tree.RemoveAndReleaseByKey(root, x);
    }
    if (root == 0) {
      cout << "Wrong!" << endl;
    } else {
      int64_t s = bst::Median<TNode, int64_t>(root);
      if (s & 1)
        cout << s / 2.0 << endl;
      else
        cout << s / 2 << endl;
    }
  }
  return 0;
}
