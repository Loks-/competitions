#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/insert_by_key.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/remove_push_down.h"
#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/tree.h"
#include "common/binary_search_tree/info/rbt_color.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/nodes_manager_fixed_size.h"

#include <vector>

namespace bst {
template <class TData, class TInfo = info::Size, class TAction = action::None,
          class TKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class RedBlackTree
    : public base::Tree<
          TTNodesManager<base::Node<TData, info::RBTColor<TInfo>, TAction, true,
                                    true, TKey>>,
          RedBlackTree<TData, TInfo, TAction, TKey, TTNodesManager>> {
 public:
  using TNode =
      base::Node<TData, info::RBTColor<TInfo>, TAction, true, true, TKey>;
  using TSelf = RedBlackTree<TData, TInfo, TAction, TKey, TTNodesManager>;
  using TTree = base::Tree<TTNodesManager<TNode>, TSelf>;
  friend TTree;

 public:
  explicit RedBlackTree(size_t max_nodes) : TTree(max_nodes) {}

 protected:
  static void BuildTreeIFixColorsR(TNode* root, size_t height) {
    assert(root || !height);
    if (!root) return;
    if (height) {
      root->info.black = true;
      BuildTreeIFixColorsR(root->l, height - 1);
      BuildTreeIFixColorsR(root->r, height - 1);
    } else {
      root->info.black = false;
      assert(!root->l && !root->r);
    }
  }

 public:
  static TNode* BuildTree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    size_t h = 0;
    for (; nodes.size() >= (1ull << h);) ++h;
    TNode* root = TTree::BuildTree(nodes);
    BuildTreeIFixColorsR(root, h - 1);
    root->info.black = true;
    return root;
  }

  static TNode* InsertByKey(TNode* root, TNode* node) {
    assert(node);
    if (!root) {
      node->info.black = true;
      return node;
    }
    base::InsertByKey<TNode>(root, node);
    node->info.black = false;
    for (;;) {
      TNode* parent = node->p;
      if (!parent) {
        node->info.black = true;
        return node;
      }
      if (parent->info.black) return root;
      TNode* gparent = parent->p;
      TNode* uncle = base::Sibling(parent, gparent);
      if (!uncle || uncle->info.black) {
        bool rotate_required = ((gparent->l == parent) != (parent->l == node));
        if (rotate_required) {
          base::RotateUp<TNode, false, false>(node);
          parent = node;
        }
        base::RotateUp<TNode, true, false>(parent);
        gparent->info.black = false;
        parent->info.black = true;
        return parent->p ? root : parent;
      }
      parent->info.black = true;
      uncle->info.black = true;
      gparent->info.black = false;
      node = gparent;
    }
    assert(false);
    return nullptr;
  }

 protected:
  static TNode* RemoveByNodeI(TNode* node) {
    base::RemovePushDown<TNode, false>(node);
    bool black = node->info.black;

    // Drop node from tree
    TNode* child = node->l ? node->l : node->r;
    TNode* parent = node->p;
    if (parent) {
      if (parent->l == node)
        parent->l = child;
      else
        parent->r = child;
    }
    if (child) child->p = parent;
    node->ResetLinksAndUpdateInfo();
    info::UpdateNodeToRoot(parent);

    // Fix colors
    if (!black) return (parent ? base::Root(parent) : child);
    for (;;) {
      if (child && !child->info.black) {
        child->info.black = true;
        return base::Root(child);
      }
      if (!parent) return child;
      TNode* sibling = base::Sibling(child, parent);
      assert(sibling);
      sibling->ApplyAction();
      if (!sibling->info.black) {
        assert(parent->info.black);
        base::RotateUp<TNode, true, false>(sibling);
        sibling->info.black = true;
        parent->info.black = false;
        sibling = base::Sibling(child, parent);
        sibling->ApplyAction();
      }
      assert(sibling && sibling->info.black);
      if (parent->info.black && (!sibling->l || sibling->l->info.black) &&
          (!sibling->r || sibling->r->info.black)) {
        sibling->info.black = false;
        child = parent;
        parent = child->p;
        continue;
      }
      if (!parent->info.black && (!sibling->l || sibling->l->info.black) &&
          (!sibling->r || sibling->r->info.black)) {
        sibling->info.black = false;
        parent->info.black = true;
        return base::Root(parent);
      }
      if ((parent->l == child) && (!sibling->r || sibling->r->info.black)) {
        assert(sibling->l && !sibling->l->info.black);
        base::RotateUp<TNode, false, true>(sibling->l);
        sibling->info.black = false;
        sibling = sibling->p;
        sibling->info.black = true;
      } else if ((parent->r == child) &&
                 (!sibling->l || sibling->l->info.black)) {
        assert(sibling->r && !sibling->r->info.black);
        base::RotateUp<TNode, false, true>(sibling->r);
        sibling->info.black = false;
        sibling = sibling->p;
        sibling->info.black = true;
      }
      sibling->info.black = parent->info.black;
      parent->info.black = true;
      if (parent->l == child)
        sibling->r->info.black = true;
      else
        sibling->l->info.black = true;
      base::RotateUp<TNode, true, false>(sibling);
      return base::Root(sibling);
    }
    assert(false);
    return nullptr;
  }
};
}  // namespace bst
