// https://www.hackerrank.com/challenges/the-chosen-one

#include "common/binary_search_tree/info/gcd.h"
#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTreap = bst::Treap<false, true, uint64_t,
                          bst::info::GCD<uint64_t, bst::info::None>>;
using TNode = TTreap::TNode;

int main_the_chosen_one__treap() {
  unsigned n;
  cin >> n;
  vector<uint64_t> v = ReadVector<uint64_t>(n);
  if (n == 1) {
    cout << v[0] + 1 << endl;
    return 0;
  }
  TTreap treap(n);
  TNode* root = treap.Build(v);
  for (unsigned i = 0; i < n; ++i) {
    TNode* node = treap.NodeByRawIndex(i);
    uint64_t value = node->data;
    node->data = 0;
    UpdateInfoNodeToRoot(node);
    uint64_t gcd = root->info.gcd;
    if ((value % gcd) != 0) {
      cout << gcd << endl;
      return 0;
    }
    node->data = value;
    UpdateInfoNodeToRoot(node);
  }
  return 0;
}
