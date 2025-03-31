#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/extended_tree.h"
#include "common/binary_search_tree/base/insert_by_key.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/remove_push_down.h"
#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/rbt_color.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <tuple>
#include <vector>

namespace bst {

template <class TData, class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t,
          template <class> class TTNodesManager =
              memory::ContiguousNodesManager>
class RedBlackTree
    : public base::ExtendedTree<
          TTNodesManager<
              base::Node<TData,
                         base::SubtreeData<templates::PrependT<
                             subtree_data::RBTColor, TAggregatorsTuple>>,
                         base::Deferred<TDeferredTuple>, true, true, TKey>>,
          RedBlackTree<TData, TAggregatorsTuple, TDeferredTuple, TKey,
                       TTNodesManager>> {
 public:
  using TSubtreeData = base::SubtreeData<
      templates::PrependT<subtree_data::RBTColor, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode = base::Node<TData, TSubtreeData, base::Deferred<TDeferredTuple>,
                           true, true, TKey>;
  using TSelf = RedBlackTree<TData, TAggregatorsTuple, TDeferredTuple, TKey,
                             TTNodesManager>;
  using TTree = base::ExtendedTree<TTNodesManager<TNode>, TSelf>;
  friend TTree;

  static constexpr bool support_join3 = true;
  static constexpr bool support_join = support_join3;

 protected:
  static constexpr bool IsBlack(const TNode* node) {
    return !node || subtree_data::RBTColor::get(node);
  }

  static constexpr bool IsRed(const TNode* node) {
    return node && !subtree_data::RBTColor::get(node);
  }

  static constexpr void SetColor(TNode* node, bool is_black) {
    assert(node);
    subtree_data::RBTColor::set(node, is_black);
  }

 public:
  explicit RedBlackTree(size_t max_nodes) : TTree(max_nodes) {}

 protected:
  static void BuildTreeIFixColorsR(TNode* root, size_t height) {
    assert(root || !height);
    if (!root) return;
    if (height) {
      SetColor(root, true);
      BuildTreeIFixColorsR(root->left, height - 1);
      BuildTreeIFixColorsR(root->right, height - 1);
    } else {
      SetColor(root, false);
      assert(!root->left && !root->right);
    }
  }

 public:
  static TNode* build_tree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    size_t h = 0;
    for (; nodes.size() >= (1ull << h);) ++h;
    TNode* root = TTree::build_tree(nodes);
    BuildTreeIFixColorsR(root, h - 1);
    SetColor(root, true);
    return root;
  }

  static TNode* insert(TNode* root, TNode* node) {
    assert(node);
    if (!root) {
      SetColor(node, true);
      return node;
    }
    base::InsertByKey<TNode>(root, node);
    SetColor(node, false);
    for (;;) {
      TNode* parent = node->parent;
      if (!parent) {
        SetColor(node, true);
        return node;
      }
      if (IsBlack(parent)) return root;
      TNode* gparent = parent->parent;
      TNode* uncle = base::sibling(parent, gparent);
      if (IsBlack(uncle)) {
        const bool rotate_required =
            ((gparent->left == parent) != (parent->left == node));
        if (rotate_required) {
          base::rotate_up<false, false>(node);
          parent = node;
        }
        base::rotate_up<true, false>(parent);
        SetColor(gparent, false);
        SetColor(parent, true);
        return parent->parent ? root : parent;
      }
      SetColor(parent, true);
      SetColor(uncle, true);
      SetColor(gparent, false);
      node = gparent;
    }
    assert(false);
    return nullptr;
  }

 protected:
  static TNode* remove_node_impl(TNode* node) {
    base::RemovePushDown<TNode, false>(node);
    const bool black = IsBlack(node);

    // Drop node from tree
    TNode* child = node->left ? node->left : node->right;
    TNode* parent = node->parent;
    if (parent) {
      if (parent->left == node)
        parent->left = child;
      else
        parent->right = child;
    }
    if (child) child->parent = parent;
    node->reset_links_and_update_subtree_data();
    subtree_data::propagate_to_root(parent);

    // Fix colors
    if (!black) return (parent ? base::root(parent) : child);
    for (;;) {
      if (IsRed(child)) {
        SetColor(child, true);
        return base::root(child);
      }
      if (!parent) return child;
      TNode* sibling = base::sibling(child, parent);
      assert(sibling);
      sibling->apply_deferred();
      if (!IsBlack(sibling)) {
        assert(IsBlack(parent));
        base::rotate_up<true, false>(sibling);
        SetColor(sibling, true);
        SetColor(parent, false);
        sibling = base::sibling(child, parent);
        sibling->apply_deferred();
      }
      assert(sibling && IsBlack(sibling));
      if (IsBlack(parent) && IsBlack(sibling->left) &&
          IsBlack(sibling->right)) {
        SetColor(sibling, false);
        child = parent;
        parent = child->parent;
        continue;
      }
      if (!IsBlack(parent) && IsBlack(sibling->left) &&
          IsBlack(sibling->right)) {
        SetColor(sibling, false);
        SetColor(parent, true);
        return base::root(parent);
      }
      if ((parent->left == child) && IsBlack(sibling->right)) {
        assert(IsRed(sibling->left));
        base::rotate_up<false, true>(sibling->left);
        SetColor(sibling, false);
        sibling = sibling->parent;
        SetColor(sibling, true);
      } else if ((parent->right == child) && IsBlack(sibling->left)) {
        assert(IsRed(sibling->right));
        base::rotate_up<false, true>(sibling->right);
        SetColor(sibling, false);
        sibling = sibling->parent;
        SetColor(sibling, true);
      }
      SetColor(sibling, IsBlack(parent));
      SetColor(parent, true);
      if (parent->left == child) {
        SetColor(sibling->right, true);
      } else {
        SetColor(sibling->left, true);
      }
      base::rotate_up<true, false>(sibling);
      return base::root(sibling);
    }
    assert(false);
    return nullptr;
  }

 public:
  static TNode* RemoveRight(TNode* root, TNode*& removed_node) {
    assert(root);
    auto node = root;
    for (node->apply_deferred(); node->right; node->apply_deferred())
      node = node->right;
    removed_node = node;
    return remove_node_impl(removed_node);
  }

  static TNode* join(TNode* l, TNode* r) {
    if (!l) return r;
    if (!r) return l;
    TNode* node = nullptr;
    l = RemoveRight(l, node);
    return join3(l, node, r);
  }

 protected:
  static int BHeight(TNode* root) {
    int h = 0;
    for (; root; root = root->left) {
      root->apply_deferred();
      if (IsBlack(root)) ++h;
    }
    return h;
  }

  static TNode* join3_impl(TNode* l, TNode* m1, TNode* r) {
    TTree::join3_impl(l, m1, r);
    SetColor(m1, false);
    return m1;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, int hd) {
    if (IsBlack(l) && (hd == 0)) return join3_impl(l, m1, r);
    l->apply_deferred();
    l->set_right(Join3L(l->right, m1, r, hd - (IsBlack(l) ? 1 : 0)));
    r = l->right;
    if (IsBlack(l) && !IsBlack(r) && IsRed(r->right)) {
      SetColor(r->right, true);
      base::rotate<true, false, TNode>(r, l, l->parent);
      return r;
    } else {
      l->update_subtree_data();
      return l;
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, int hd) {
    if (IsBlack(r) && (hd == 0)) return join3_impl(l, m1, r);
    r->apply_deferred();
    r->set_left(Join3R(l, m1, r->left, hd - (IsBlack(r) ? 1 : 0)));
    l = r->left;
    if (IsBlack(r) && !IsBlack(l) && IsRed(l->left)) {
      SetColor(l->left, true);
      base::rotate<true, false, TNode>(l, r, r->parent);
      return l;
    } else {
      r->update_subtree_data();
      return r;
    }
  }

 public:
  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->left && !m1->right);
    const auto hl = BHeight(l), hr = BHeight(r), hd = hl - hr;
    auto root = (hd > 0)   ? Join3L(l, m1, r, hd)
                : (hd < 0) ? Join3R(l, m1, r, -hd)
                           : join3_impl(l, m1, r);
    SetColor(root, true);
    return root;
  }
};
}  // namespace bst
