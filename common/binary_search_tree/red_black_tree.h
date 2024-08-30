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
#include "common/memory/nodes_manager_fixed_size.h"

#include <vector>

namespace bst {
template <class TData, class TInfo = info::Size, class TAction = action::None,
          class TKey = int64_t,
          template <class> class TTNodesManager = memory::NodesManagerFixedSize>
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

  static constexpr bool support_join3 = true;
  static constexpr bool support_join = support_join3;

 protected:
  static constexpr bool IsBlack(const TNode* node) {
    return !node || node->info.black;
  }
  static constexpr bool IsRed(const TNode* node) {
    return node && !node->info.black;
  }

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
      if (IsBlack(uncle)) {
        const bool rotate_required =
            ((gparent->l == parent) != (parent->l == node));
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
    const bool black = node->info.black;

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
      if (IsRed(child)) {
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
      if (parent->info.black && IsBlack(sibling->l) && IsBlack(sibling->r)) {
        sibling->info.black = false;
        child = parent;
        parent = child->p;
        continue;
      }
      if (!parent->info.black && IsBlack(sibling->l) && IsBlack(sibling->r)) {
        sibling->info.black = false;
        parent->info.black = true;
        return base::Root(parent);
      }
      if ((parent->l == child) && IsBlack(sibling->r)) {
        assert(IsRed(sibling->l));
        base::RotateUp<TNode, false, true>(sibling->l);
        sibling->info.black = false;
        sibling = sibling->p;
        sibling->info.black = true;
      } else if ((parent->r == child) && IsBlack(sibling->l)) {
        assert(IsRed(sibling->r));
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

 public:
  static TNode* RemoveRight(TNode* root, TNode*& removed_node) {
    assert(root);
    auto node = root;
    for (node->ApplyAction(); node->r; node->ApplyAction()) node = node->r;
    removed_node = node;
    return RemoveByNodeI(removed_node);
  }

  static TNode* Join(TNode* l, TNode* r) {
    if (!l) return r;
    if (!r) return l;
    TNode* node = nullptr;
    l = RemoveRight(l, node);
    return Join3(l, node, r);
  }

 protected:
  static int BHeight(TNode* root) {
    int h = 0;
    for (; root; root = root->l) {
      root->ApplyAction();
      if (root->info.black) ++h;
    }
    return h;
  }

  static TNode* Join3IBase(TNode* l, TNode* m1, TNode* r) {
    TTree::Join3IBase(l, m1, r);
    m1->info.black = false;
    return m1;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, int hd) {
    if (IsBlack(l) && (hd == 0)) return Join3IBase(l, m1, r);
    l->ApplyAction();
    l->SetR(Join3L(l->r, m1, r, hd - (l->info.black ? 1 : 0)));
    r = l->r;
    if (l->info.black && !r->info.black && IsRed(r->r)) {
      r->r->info.black = true;
      base::Rotate<TNode, true, false>(r, l, l->p);
      return r;
    } else {
      l->UpdateInfo();
      return l;
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, int hd) {
    if (IsBlack(r) && (hd == 0)) return Join3IBase(l, m1, r);
    r->ApplyAction();
    r->SetL(Join3R(l, m1, r->l, hd - (r->info.black ? 1 : 0)));
    l = r->l;
    if (r->info.black && !l->info.black && IsRed(l->l)) {
      l->l->info.black = true;
      base::Rotate<TNode, true, false>(l, r, r->p);
      return l;
    } else {
      r->UpdateInfo();
      return r;
    }
  }

 public:
  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->l && !m1->r);
    const auto hl = BHeight(l), hr = BHeight(r), hd = hl - hr;
    auto root = (hd > 0)   ? Join3L(l, m1, r, hd)
                : (hd < 0) ? Join3R(l, m1, r, -hd)
                           : Join3IBase(l, m1, r);
    root->info.black = true;
    return root;
  }
};
}  // namespace bst
