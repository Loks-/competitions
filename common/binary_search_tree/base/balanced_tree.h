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

  static const bool support_join = TMe::support_join3;

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

 public:
  static TNode* RemoveRight(TNode* root, TNode*& removed_node) {
    assert(root);
    root->ApplyAction();
    if (root->r) {
      root->SetR(RemoveRight(root->r, removed_node));
      root->UpdateInfo();
      return TMe::FixBalanceRemove(root);
    } else {
      removed_node = root;
      TNode* node = root->l;
      if (node) node->SetP(nullptr);
      root->ResetLinksAndUpdateInfo();
      return node;
    }
  }

 protected:
  static TNode* JoinI(TNode* l, TNode* r) {
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
