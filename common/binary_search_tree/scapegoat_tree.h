#pragma once

#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/info/size.h"
#include "common/memory/nodes_manager_fixed_size.h"

namespace bst {
// In this implementation delete operation is different from wiki Scapegoat
// tree. It removes node from tree (similar to other trees), not just mark for
// future deletion.
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class ScapegoatTree
    : public base::BalancedTree<
          memory::NodesManagerFixedSize<
              base::Node<TData, TInfo, TAction, true, use_parent, TKey>>,
          ScapegoatTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  // Height is less or equal to 2 * height(fully balanced tree).
  static constexpr double alpha = 0.7;

  using TNode = base::Node<TData, TInfo, TAction, true, use_parent, TKey>;
  using TSelf = ScapegoatTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree =
      base::BalancedTree<memory::NodesManagerFixedSize<TNode>, TSelf>;
  using TTree = typename TBTree::TTree;
  friend TBTree;

  // Split/Join is supported but it's slow.
  static constexpr bool support_join3 = true;

 public:
  constexpr explicit ScapegoatTree(size_t max_nodes) : TBTree(max_nodes) {}

  static size_t Size(const TNode* root) { return root ? root->info.size : 0; }

 protected:
  static void TraverseInorder(TNode* node, std::vector<TNode*>& output) {
    if (!node) return;
    node->ApplyAction();
    TraverseInorder(node->l, output);
    output.push_back(node);
    TraverseInorder(node->r, output);
  }

  static TNode* RebuildSubtree(TNode* node) {
    assert(node);
    std::vector<TNode*> nodes;
    TraverseInorder(node, nodes);
    return TTree::BuildTree(nodes);
  }

  static TNode* FixBalance(TNode* node) {
    assert(node);
    const auto s = size_t(alpha * Size(node));
    return ((Size(node->l) > s) || (Size(node->r) > s)) ? RebuildSubtree(node)
                                                        : node;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, size_t rsize) {
    if (l) l->ApplyAction();
    if (Size(l) > 2 * rsize) {
      l->SetR(Join3L(l->r, m1, r, rsize));
      l->UpdateInfo();
      return FixBalance(l);
    } else {
      return TTree::Join3IBase(l, m1, r);
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, size_t lsize) {
    if (r) r->ApplyAction();
    if (Size(r) > 2 * lsize) {
      r->SetL(Join3R(l, m1, r->l, lsize));
      r->UpdateInfo();
      return FixBalance(r);
    } else {
      return TTree::Join3IBase(l, m1, r);
    }
  }

 public:
  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->l && !m1->r);
    const size_t lsize = Size(l), rsize = Size(r);
    return lsize >= rsize ? Join3L(l, m1, r, rsize) : Join3R(l, m1, r, lsize);
  }
};
}  // namespace bst
