#include "common/binary_search_tree/action/reverse.h"
#include "common/binary_search_tree/base/traversal.h"
#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"

#include <functional>

using TTree =
    bst::Treap<false, true, unsigned, bst::info::Size, bst::action::Reverse>;
using TNode = TTree::TNode;

int main_reversort_engineering() {
  unsigned T, N, C;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> C;
    ++C;
    if ((C < N) || (N * (N + 1) < 2 * C)) {
      cout << "Case #" << it + 1 << ": "
           << "IMPOSSIBLE" << endl;
    } else {
      TTree tree(N);

      std::function<TNode*(unsigned, unsigned)> Solve = [&](
          unsigned n, unsigned c) -> TNode* {
        auto node = tree.New(N - n + 1);
        if (n == 1) {
          assert(c == 1);
          return node;
        } else {
          if (c >= 2 * n - 1) {
            auto r = Solve(n - 1, c - n);
            r->AddAction(true);
            return tree.Join(node, r);
          } else {
            auto r = Solve(n - 1, c - 1);
            return tree.Join(r, node);
          }
        }
      };

      auto r = Solve(N, C);
      auto v = bst::base::Traverse<TNode, TNode::TData>(
          r, bst::base::ETraversalOrder::Inorder);
      std::reverse(v.begin(), v.end());
      cout << "Case #" << it + 1 << ":";
      for (unsigned u : v) std::cout << " " << u;
      cout << endl;
    }
  }
  return 0;
}
