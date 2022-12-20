#include "common/binary_search_tree/base/order.h"
#include "common/binary_search_tree/treap.h"
#include "common/modular/arithmetic.h"
#include "common/stl/base.h"
#include "common/vector/read_all.h"

using TTree = bst::Treap<false, true, int64_t>;
using TNode = TTree::TNode;

int main_2220() {
  auto v = nvector::ReadAll<int64_t>();
  for (bool ab : {false, true}) {
    int64_t n = v.size();
    TTree tree(v.size());
    vector<TNode *> nodes;
    for (auto x : v) nodes.push_back(tree.New(x * (ab ? 811589153 : 1)));
    auto root = tree.BuildTree(nodes);
    for (unsigned itt = 0; itt < (ab ? 10 : 1); ++itt) {
      for (auto node : nodes) {
        int64_t p = bst::base::Order(node);
        TNode *l, *t, *m, *r;
        tree.SplitBySize(root, p, l, t);
        tree.SplitBySize(t, 1, m, r);
        assert(m == node);
        root = tree.Join(l, r);
        auto pnew = modular::TArithmetic_C32U::ApplyS(p + node->data, n - 1);
        tree.SplitBySize(root, pnew, l, r);
        root = tree.Join3(l, m, r);
      }
    }
    TNode *p0 = nullptr;
    for (auto node : nodes) {
      if (node->data == 0) p0 = node;
    }
    int64_t p = bst::base::Order(p0);
    int64_t r = 0;
    for (int64_t i = 1; i <= 3; ++i) {
      auto pnew = (p + i * 1000) % n;
      auto pp = tree.FindByOrder(root, pnew);
      r += pp->data;
    }
    cout << r << endl;
  }
  return 0;
}
