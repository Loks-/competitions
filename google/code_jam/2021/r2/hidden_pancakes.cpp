#include "common/binary_search_tree/action/add_each__min.h"
#include "common/binary_search_tree/info/min.h"
#include "common/binary_search_tree/treap.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/full.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"

using TTree = bst::Treap<true, true, int, bst::info::Min<int, bst::info::Size>,
                         bst::action::AddEachMin<int>, int>;
using TNode = TTree::TNode;

int main_hidden_pancakes() {
  unsigned T, N;
  modular::mstatic::Factorial<TModularD> f;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    TTree tree(N);
    auto v = nvector::Read<int>(N);
    auto root = tree.Build(v, nvector::Enumerate<int>(0, N));
    TModularD r = 1;
    std::vector<TNode *> vtasks(1, root);
    for (unsigned i = 0; i < vtasks.size(); ++i) {
      auto node = vtasks[i];
      if (!node) continue;
      node->ApplyAction();
      if (node->info.min != 1) {
        r = 0;
        break;
      }
      if (node->info.size == 1) continue;
      auto node2 = node;
      for (; node2;) {
        node2->ApplyAction();
        if (node2->r) node2->r->ApplyAction();
        if (node2->r && (node2->r->info.min == 1)) {
          node2 = node2->r;
        } else if (node2->data == 1) {
          break;
        } else {
          node2 = node2->l;
        }
      }
      if (!node2) {
        r = 0;
        break;
      }
      assert(node2->data == 1);
      auto k = node2->key;
      TNode *t0, *t1, *t2, *t3;
      tree.SplitByKey(node, k, t0, t1);
      tree.SplitByKey(t1, k + 1, t2, t3);
      assert(t2 == node2);
      unsigned sl = t0 ? t0->info.size : 0u;
      unsigned sr = t3 ? t3->info.size : 0u;
      r *= f.BinomialCoefficient(sl + sr, sl);
      if (t3) t3->AddAction(-1);
      vtasks.push_back(t0);
      vtasks.push_back(t3);
    }

    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
