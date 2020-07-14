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

 public:
  explicit BalancedTree(size_t max_nodes) : TTree(max_nodes) {}

 protected:
  static TNode* FixBalance(TNode* node);

  static TNode* FixBalanceInsert(TNode* node) { return TMe::FixBalance(node); }
  static TNode* FixBalanceRemove(TNode* node) { return TMe::FixBalance(node); }

  static TNode* InsertByKeyIR(TNode* root, TNode* node) {
    if (!root) return node;
    root->ApplyAction();
    if (root->key < node->key)
      root->SetR(InsertByKeyIR(root->r, node));
    else
      root->SetL(InsertByKeyIR(root->l, node));
    root->UpdateInfo();
    return TMe::FixBalanceInsert(root);
  }

  static TNode* InsertByKeyI(TNode* root, TNode* node) {
    return TMe::InsertByKeyIR(root, node);
  }

  static TNode* RemoveByNodeI(TNode* node) {
    TNode *new_root, *fcn = nullptr;
    new_root = bst::base::RemoveByNode<TNode, false>(node, fcn);
    if (!fcn) return new_root;
    for (TNode* p = fcn->p; p; p = (fcn = p)->p) {
      TNode* t = TMe::FixBalanceRemove(fcn);
      if (t != fcn) {
        if (fcn == p->l)
          p->SetL(t);
        else
          p->SetR(t);
      }
    }
    return TMe::FixBalanceRemove(fcn);
  }
};
}  // namespace base
}  // namespace bst
