// https://www.hackerrank.com/challenges/heavy-light-2-white-falcon

#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum.h"
#include "common/graph/tree.h"
#include "common/graph/tree/link_cut_tree.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/stl/pair.h"

namespace {
class ActionAddArithmeticSequence : public bst::action::Reverse {
 public:
  using TBase = bst::action::Reverse;
  using TSelf = ActionAddArithmeticSequence;
  using TActionValue = std::pair<TModularD, TModularD>;

  TActionValue value;

  ActionAddArithmeticSequence() : value(0, 0) {}

  bool IsValueEmpty() const {
    return (value.first == 0) && (value.second == 0);
  }

  bool IsEmpty() const { return IsValueEmpty() && TBase::IsEmpty(); }

  void ClearValue() { value = std::make_pair(0, 0); }

  void Clear() {
    ClearValue();
    TBase::Clear();
  }

  template <class TNode>
  void ReverseSubtree(TNode* node) {
    TBase::ReverseSubtree(node);
    if (!IsValueEmpty()) {
      value.first += value.second * (node->info.size - 1);
      value.second = -value.second;
    }
  }

  template <class TNode>
  void Add(TNode* node, TActionValue add_value) {
    TModularD s = node->info.size;
    node->info.sum += add_value.first * s + add_value.second * s * (s - 1) / 2;
    value += add_value;
  }

  template <class TNode>
  void Apply(TNode* node) {
    TBase::Apply(node);
    if (!IsValueEmpty()) {
      TModularD lsize = (node->l ? node->l->info.size : 0);
      node->data += value.first + value.second * lsize;
      if (node->l) node->l->AddAction(value);
      if (node->r)
        node->r->AddAction(std::make_pair(
            value.first + value.second * (lsize + 1), value.second));
      ClearValue();
    }
  }
};
}  // namespace

int main_heavy_light_2_white_falcon__lct() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  graph::LinkCutTree<TModularD, bst::info::Sum<TModularD, bst::info::Size>,
                     ActionAddArithmeticSequence>
      lct(tree);

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, v, x;
    cin >> t >> u >> v;
    if (t == 1) {
      cin >> x;
      lct.AddActionOnPath(u, v, make_pair<TModularD, TModularD>(x, x));
    } else if (t == 2) {
      cout << lct.GetPathInfo(u, v).sum << endl;
    }
  }
  return 0;
}
