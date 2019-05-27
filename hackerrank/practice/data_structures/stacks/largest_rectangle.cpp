// https://www.hackerrank.com/challenges/largest-rectangle

#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"

using TTreap = bst::Treap<false, false, TEmpty>;
using TNode = TTreap::TNode;

int main_largest_rectangle() {
  unsigned N, h;
  cin >> N;
  TTreap tree(N);
  vector<TNode*> vnodes(N);
  for (unsigned i = 0; i < N; ++i) {
    cin >> h;
    vnodes[i] = tree.New();
    vnodes[i]->height = ~h;
  }
  tree.BuildTree(vnodes);
  uint64_t output = 0;
  for (TNode* p : vnodes)
    output = max(output, unsigned(~p->height) * uint64_t(p->info.size));
  cout << output << endl;
  return 0;
}
