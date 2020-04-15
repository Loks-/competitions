#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_action.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/insert_by_key.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/swap.h"
#include "common/binary_search_tree/info/rbt_color.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/binary_search_tree/info/update_node_to_root_with_path.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace bst {
template <bool _use_parent, class TTData, class TTInfo = info::Size,
          class TTAction = action::None, class TTKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class RedBlackTree
    : public Tree<TTNodesManager<Node<TTData, info::RBTColor<TTInfo>, TTAction,
                                      true, _use_parent, false, TTKey>>,
                  RedBlackTree<_use_parent, TTData, TTInfo, TTAction, TTKey,
                               TTNodesManager>> {
 public:
  static const bool use_key = true;
  static const bool use_parent = _use_parent;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = info::RBTColor<TTInfo>;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf =
      RedBlackTree<use_parent, TData, TTInfo, TAction, TKey, TTNodesManager>;
  using TTree = Tree<TTNodesManager<TNode>, TSelf>;
  friend class Tree<TTNodesManager<TNode>, TSelf>;

 public:
  RedBlackTree(size_t max_nodes) : TTree(max_nodes) {}

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

 protected:
  static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeFalse) {
    thread_local std::vector<TNode*> node_to_root_path;
    if (!root) {
      node->info.black = true;
      return node;
    }
    node_to_root_path.clear();
    for (TNode* current = root;;) {
      current->ApplyAction();
      node_to_root_path.push_back(current);
      if (current->key < node->key) {
        if (current->r) {
          current = current->r;
        } else {
          current->SetR(node);
          break;
        }
      } else {
        if (current->l) {
          current = current->l;
        } else {
          current->SetL(node);
          break;
        }
      }
    }
    node_to_root_path.push_back(node);
    std::reverse(node_to_root_path.begin(), node_to_root_path.end());
    info::UpdateNodeToRootWithPath(node_to_root_path, 1);
    node_to_root_path.push_back(nullptr);
    node->info.black = false;
    for (size_t node_index = 1;;) {
      TNode* parent = node_to_root_path[node_index++];
      if (!parent) {
        node->info.black = true;
        return node;
      }
      if (parent->info.black) return root;
      TNode* gparent = node_to_root_path[node_index++];
      TNode* uncle = Sibling(parent, gparent);
      if (!uncle || uncle->info.black) {
        bool rotate_required = ((gparent->l == parent) != (parent->l == node));
        if (rotate_required) {
          Rotate<TNode, false, false>(node, parent, gparent);
          parent = node;
        }
        TNode* ggparent = node_to_root_path[node_index++];
        Rotate<TNode, true, false>(parent, gparent, ggparent);
        gparent->info.black = false;
        parent->info.black = true;
        return ggparent ? root : parent;
      }
      gparent->info.black = false;
      parent->info.black = true;
      uncle->info.black = true;
      node = gparent;
    }
    assert(false);
    return nullptr;
  }

  static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeTrue) {
    bst::InsertByKey<TNode>(root, node);
    node->info.black = false;
    for (;;) {
      TNode* parent = node->p;
      if (!parent) {
        node->info.black = true;
        return node;
      }
      if (parent->info.black) return root;
      TNode* gparent = parent->p;
      TNode* uncle = Sibling(parent, gparent);
      if (!uncle || uncle->info.black) {
        bool rotate_required = ((gparent->l == parent) != (parent->l == node));
        if (rotate_required) {
          RotateUp<TNode, false, false>(node);
          parent = node;
        }
        RotateUp<TNode, true, false>(parent);
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

 public:
  static TNode* InsertByKey(TNode* root, TNode* node) {
    assert(node);
    return InsertByKeyI(root, node, TFakeBool<use_parent>());
  }

 protected:
  static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                             TFakeFalse) {
    thread_local std::vector<TNode*> node_to_root_path;
    // Find node
    if (!root) return root;
    node_to_root_path.clear();
    TNode* node = root;
    for (; node;) {
      node->ApplyAction();
      node_to_root_path.push_back(node);
      if (node->key < key) {
        node = node->r;
      } else if (node->key > key) {
        node = node->l;
      } else {
        removed_node = node;
        break;
      }
    }
    if (!node) return root;

    // Optional swap
    if (node->l && node->r) {
      size_t node_index = node_to_root_path.size() - 1;
      TNode* temp = node->l;
      for (temp->ApplyAction(); temp->r;) {
        node_to_root_path.push_back(temp);
        temp = temp->r;
        temp->ApplyAction();
      }
      SwapAuto(node,
               (node_index > 0) ? node_to_root_path[node_index - 1] : nullptr,
               temp, node_to_root_path.back());
      std::swap(node->info.black, temp->info.black);
      node_to_root_path[node_index] = temp;
      node_to_root_path.push_back(node);
      root = node_to_root_path[0];
    }

    // Drop node from tree
    std::reverse(node_to_root_path.begin(), node_to_root_path.end());
    TNode* child = node->l ? node->l : node->r;
    TNode* parent =
        node_to_root_path.size() > 1 ? node_to_root_path[1] : nullptr;
    if (parent) {
      if (parent->l == node)
        parent->SetL(child);
      else
        parent->SetR(child);
    } else if (child) {
      child->SetP(nullptr);
    }
    node->ResetLinksAndUpdateInfo();
    info::UpdateNodeToRootWithPath(node_to_root_path, 1);

    // Fix colors
    node_to_root_path.push_back(nullptr);
    size_t current_index = 2;
    if (!node->info.black) return (parent ? root : child);
    for (;;) {
      if (child && !child->info.black) {
        child->info.black = true;
        return (parent ? root : child);
      }
      if (!parent) return child;
      TNode* sibling = Sibling(child, parent);
      assert(sibling);
      sibling->ApplyAction();
      TNode* gparent = node_to_root_path[current_index];
      if (!sibling->info.black) {
        assert(parent->info.black);
        Rotate<TNode, true, false>(sibling, parent, gparent);
        node_to_root_path[--current_index] = sibling;
        if (!gparent) root = sibling;
        gparent = sibling;
        sibling->info.black = true;
        parent->info.black = false;
        sibling = Sibling(child, parent);
        sibling->ApplyAction();
      }
      assert(sibling && sibling->info.black);
      if (parent->info.black && (!sibling->l || sibling->l->info.black) &&
          (!sibling->r || sibling->r->info.black)) {
        sibling->info.black = false;
        child = parent;
        parent = node_to_root_path[current_index++];
        continue;
      }
      if (!parent->info.black && (!sibling->l || sibling->l->info.black) &&
          (!sibling->r || sibling->r->info.black)) {
        sibling->info.black = false;
        parent->info.black = true;
        return root;
      }
      if ((parent->l == child) && (!sibling->r || sibling->r->info.black)) {
        assert(sibling->l && !sibling->l->info.black);
        Rotate<TNode, false, true>(sibling->l, sibling, parent);
        sibling->info.black = false;
        sibling = parent->r;
        sibling->info.black = true;
      } else if ((parent->r == child) &&
                 (!sibling->l || sibling->l->info.black)) {
        assert(sibling->r && !sibling->r->info.black);
        Rotate<TNode, false, true>(sibling->r, sibling, parent);
        sibling->info.black = false;
        sibling = parent->l;
        sibling->info.black = true;
      }
      sibling->info.black = parent->info.black;
      parent->info.black = true;
      if (parent->l == child)
        sibling->r->info.black = true;
      else
        sibling->l->info.black = true;
      Rotate<TNode, true, false>(sibling, parent, gparent);
      return gparent ? root : sibling;
    }
    assert(false);
    return nullptr;
  }

  static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                             TFakeTrue) {
    removed_node = TTree::FindByKey(root, key);
    return (removed_node ? RemoveByNode(removed_node) : root);
  }

 public:
  static TNode* RemoveByNode(TNode* node) {
    static_assert(use_parent, "use_parent should be true");
    assert(node);
    ApplyActionRootToNode(node);

    // Optional swap
    if (node->l && node->r) {
      TNode* temp = node->l;
      for (temp->ApplyAction(); temp->r;) {
        temp = temp->r;
        temp->ApplyAction();
      }
      SwapAuto(node, node->p, temp, temp->p);
      std::swap(node->info.black, temp->info.black);
    }

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
    if (!node->info.black) return (parent ? Root(parent) : child);
    for (;;) {
      if (child && !child->info.black) {
        child->info.black = true;
        return Root(child);
      }
      if (!parent) return child;
      TNode* sibling = Sibling(child, parent);
      assert(sibling);
      sibling->ApplyAction();
      if (!sibling->info.black) {
        assert(parent->info.black);
        RotateUp<TNode, true, false>(sibling);
        sibling->info.black = true;
        parent->info.black = false;
        sibling = Sibling(child, parent);
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
        return Root(parent);
      }
      if ((parent->l == child) && (!sibling->r || sibling->r->info.black)) {
        assert(sibling->l && !sibling->l->info.black);
        RotateUp<TNode, false, true>(sibling->l);
        sibling->info.black = false;
        sibling = sibling->p;
        sibling->info.black = true;
      } else if ((parent->r == child) &&
                 (!sibling->l || sibling->l->info.black)) {
        assert(sibling->r && !sibling->r->info.black);
        RotateUp<TNode, false, true>(sibling->r);
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
      RotateUp<TNode, true, false>(sibling);
      return Root(sibling);
    }
    assert(false);
    return nullptr;
  }
};
}  // namespace bst
