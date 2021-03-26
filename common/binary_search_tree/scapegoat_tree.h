#pragma once

#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/info/size.h"
#include "common/nodes_manager_fixed_size.h"

namespace bst {
// In this implementation delete operation is different from wiki Scapegoat
// tree. It removes node from tree (similar to other trees), not just mark for
// future deletion.
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class ScapegoatTree
    : public base::BalancedTree<
          NodesManagerFixedSize<
              base::Node<TData, TInfo, TAction, true, use_parent, TKey>>,
          ScapegoatTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  // Height is less or equal to 2 * height(fully balanced tree).
  static constexpr double alpha = 0.7;

  using TNode = base::Node<TData, TInfo, TAction, true, use_parent, TKey>;
  using TSelf = ScapegoatTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree = base::BalancedTree<NodesManagerFixedSize<TNode>, TSelf>;
  using TTree = typename TBTree::TTree;
  friend TBTree;

 public:
  explicit ScapegoatTree(size_t max_nodes) : TBTree(max_nodes) {}

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
    return ((node->l && node->l->info.size > alpha * node->info.size) ||
            (node->r && node->r->info.size > alpha * node->info.size))
               ? RebuildSubtree(node)
               : node;
  }
};
}  // namespace bst
