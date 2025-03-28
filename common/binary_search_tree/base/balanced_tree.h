#pragma once

#include "common/binary_search_tree/base/remove_by_node.h"
#include "common/binary_search_tree/base/tree.h"

namespace bst {
namespace base {
template <class TTNodesManager, class TTMe>
class BalancedTree : public Tree<TTNodesManager, TTMe> {
 public:
  using TTree = Tree<TTNodesManager, TTMe>;
  using TNode = typename TTree::TNode;
  using TMe = TTMe;
  friend TTree;

  static constexpr bool support_join = TMe::support_join3;

 public:
  explicit BalancedTree(size_t max_nodes) : TTree(max_nodes) {}

 protected:
  static TNode* FixBalance(TNode* node);

  static TNode* FixBalanceInsert(TNode* node) { return TMe::FixBalance(node); }

  static TNode* FixBalanceRemove(TNode* node) { return TMe::FixBalance(node); }

  static TNode* InsertByKeyIR(TNode* root, TNode* node) {
    if (!root) return node;
    root->apply_deferred();
    if (root->key < node->key) {
      root->set_right(InsertByKeyIR(root->right, node));
    } else {
      root->set_left(InsertByKeyIR(root->left, node));
    }
    root->update_subtree_data();
    return TMe::FixBalanceInsert(root);
  }

  static TNode* InsertByKeyI(TNode* root, TNode* node) {
    return TMe::InsertByKeyIR(root, node);
  }

  static TNode* RemoveByNodeI(TNode* node) {
    TNode *new_root, *fcn = nullptr;
    new_root = bst::base::RemoveByNode<TNode, false>(node, fcn);
    if (!fcn) return new_root;
    for (TNode* p = fcn->parent; p; p = (fcn = p)->parent) {
      TNode* t = TMe::FixBalanceRemove(fcn);
      if (t != fcn) {
        if (fcn == p->left) {
          p->set_left(t);
        } else {
          p->set_right(t);
        }
      }
    }
    return TMe::FixBalanceRemove(fcn);
  }

 public:
  static TNode* RemoveRight(TNode* root, TNode*& removed_node) {
    assert(root);
    root->apply_deferred();
    if (root->right) {
      root->set_right(RemoveRight(root->right, removed_node));
      root->update_subtree_data();
      return TMe::FixBalanceRemove(root);
    } else {
      removed_node = root;
      TNode* node = root->left;
      if (node) node->set_parent(nullptr);
      root->reset_links_and_update_subtree_data();
      return node;
    }
  }

  static TNode* Join(TNode* l, TNode* r) {
    static_assert(TMe::support_join, "Join should be supported");
    if (!l) return r;
    if (!r) return l;
    TNode* node = nullptr;
    l = TMe::RemoveRight(l, node);
    return TMe::Join3(l, node, r);
  }
};
}  // namespace base
}  // namespace bst
