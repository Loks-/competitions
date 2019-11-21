// https://www.hackerrank.com/challenges/direct-connections

#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum_keys.h"
#include "common/binary_search_tree/treap.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree = bst::Treap<true, false, TEmpty,
                         bst::info::SumKeys<uint64_t, bst::info::Size>,
                         bst::action::None, uint64_t>;
using TNode = TTree::TNode;

int main_direct_connections__treap() {
  unsigned T, N;
  cin >> T;
  TTree tree(200000);
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    vector<uint64_t> vx = ReadVector<uint64_t>(N);
    vector<uint64_t> vp = ReadVector<uint64_t>(N);
    vector<pair<uint64_t, uint64_t>> vpx;
    for (unsigned i = 0; i < N; ++i) vpx.push_back({vp[i], vx[i]});
    sort(vpx.begin(), vpx.end());
    TModularD total = 0;
    TNode *root = 0, *l, *r;
    for (auto px : vpx) {
      tree.SplitByKey(root, px.second, l, r);
      TModularD s = 0;
      if (l) s += TModularD(l->info.size * px.second - l->info.sum_keys);
      if (r) s += TModularD(r->info.sum_keys - r->info.size * px.second);
      total += s * TModularD(px.first);
      TNode* m = tree.New(TEmpty(), px.second);
      root = tree.Join(tree.Join(l, m), r);
    }
    tree.ReleaseTree(root);
    cout << total << endl;
  }
  return 0;
}
