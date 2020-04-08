#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_action.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/info/height.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

namespace bst {
template <bool _use_parent, class TTData, class TTInfo = info::Size,
          class TTAction = action::None, class TTKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class AVLTree
    : public Tree<TTNodesManager<Node<TTData, info::Height<TTInfo>, TTAction,
                                      true, _use_parent, false, TTKey>>,
                  AVLTree<_use_parent, TTData, TTInfo, TTAction, TTKey,
                          TTNodesManager>> {
 public:
  static const bool use_key = true;
  static const bool use_parent = _use_parent;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = info::Height<TTInfo>;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf =
      AVLTree<use_parent, TData, TTInfo, TAction, TKey, TTNodesManager>;
  using TTree = Tree<TTNodesManager<TNode>, TSelf>;
  friend class Tree<TTNodesManager<TNode>, TSelf>;

 public:
  AVLTree(unsigned max_nodes) : TTree(max_nodes) {}

  static int Height(TNode* node) { return node ? int(node->info.height) : 0; }

  static int Balance(TNode* node) {
    return node ? Height(node->l) - Height(node->r) : 0;
  }

 protected:
  template <bool apply_action_on_child>
  static TNode* FixBalance(TNode* root) {
    if (Balance(root) == 2) {
      if (Balance(root->l) == -1)
        Rotate<TNode, false, apply_action_on_child>(root->l->r, root->l, root);
      TNode* child = root->l;
      Rotate<TNode, true, apply_action_on_child>(child, root, nullptr);
      return child;
    } else if (Balance(root) == -2) {
      if (Balance(root->r) == 1)
        Rotate<TNode, false, apply_action_on_child>(root->r->l, root->r, root);
      TNode* child = root->r;
      Rotate<TNode, true, apply_action_on_child>(child, root, nullptr);
      return child;
    }
    root->UpdateInfo();
    return root;
  }

 public:
  static TNode* InsertByKey(TNode* root, TNode* node) {
    if (!root) return node;
    root->ApplyAction();
    if (root->key < node->key)
      root->SetR(InsertByKey(root->r, node));
    else
      root->SetL(InsertByKey(root->l, node));
    return FixBalance<false>(root);
  }

 protected:
  static TNode* SwapAndRemove(TNode* root, TNode* node) {
    root->ApplyAction();
    if (root->r) {
      root->SetR(SwapAndRemove(root->r, node));
      return FixBalance<true>(root);
    } else {
      TNode* child = root->l;
      root->SetL(node->l);
      root->SetR(node->r);
      node->l = root;  // Save information about swapped node
      return child;
    }
  }

 public:
  static TNode* RemoveByKey(TNode* root, const TKey& key,
                            TNode*& removed_node) {
    if (!root) return root;
    root->ApplyAction();
    if (root->key < key) {
      root->SetR(RemoveByKey(root->r, key, removed_node));
    } else if (root->key > key) {
      root->SetL(RemoveByKey(root->l, key, removed_node));
    } else {
      removed_node = root;
      if (root->l && root->r) {
        TNode* child = root->l;
        child->ApplyAction();
        if (child->r) {
          TNode* temp = SwapAndRemove(child, root);
          child = root->l;
          root->ResetLinksAndUpdateInfo();
          child->SetL(temp);
          return FixBalance<true>(child);
        } else {
          child->SetR(root->r);
          root->ResetLinksAndUpdateInfo();
          return FixBalance<true>(child);
        }
      } else {
        TNode* child = root->l ? root->l : root->r;
        root->ResetLinksAndUpdateInfo();
        return child;
      }
    }
    return FixBalance<true>(root);
  }

  static TNode* RemoveByNode(TNode* node) {
    static_assert(use_parent, "use_parent should be true");
    assert(node);
    ApplyActionRootToNode(node);

    TNode *parent = node->p, *child;
    if (node->l && node->r) {
      child = node->l;
      child->ApplyAction();
      if (child->r) {
        TNode* temp = SwapAndRemove(child, node);
        child = node->l;
        child->SetL(temp);
        child = FixBalance<true>(child);
      } else {
        child->SetR(node->r);
        node->ResetLinksAndUpdateInfo();
        child = FixBalance<true>(child);
      }
    } else {
      child = node->l ? node->l : node->r;
    }
    node->ResetLinksAndUpdateInfo();

    for (; parent;) {
      if (parent->l == node)
        parent->SetL(child);
      else
        parent->SetR(child);
      node = parent;
      parent = node->p;
      child = FixBalance<true>(node);
    }
    if (child) child->SetP(nullptr);
    return child;
  }
};
}  // namespace bst
