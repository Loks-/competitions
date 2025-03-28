#pragma once

#include "common/base.h"

#include <utility>

namespace bst {
namespace base {
// Swap doesn't apply action and not update info.
template <class TNode>
constexpr void SwapNotRelated(TNode* node1, TNode* parent1, TNode* node2,
                              TNode* parent2) {
  assert(node1 != node2);
  TNode *child1l = node1->left, *child1r = node1->right;
  node1->set_left(node2->left);
  node1->set_right(node2->right);
  node2->set_left(child1l);
  node2->set_right(child1r);
  if (parent1 && parent1 == parent2) {
    std::swap(parent1->left, parent1->right);
    return;
  }
  if (parent1) {
    if (node1 == parent1->left)
      parent1->set_left(node2);
    else
      parent1->set_right(node2);
  } else {
    node2->set_parent(nullptr);
  }
  if (parent2) {
    if (node2 == parent2->left)
      parent2->set_left(node1);
    else
      parent2->set_right(node1);
  } else {
    node1->set_parent(nullptr);
  }
  node1->subtree_data.bti_swap(node2);
}

template <class TNode>
constexpr void SwapChildParent(TNode* child, TNode* parent, TNode* gparent) {
  if (child == parent->left) {
    TNode* sibling = parent->right;
    parent->set_left(child->left);
    parent->set_right(child->right);
    child->set_left(parent);
    child->set_right(sibling);
  } else {
    TNode* sibling = parent->left;
    parent->set_left(child->left);
    parent->set_right(child->right);
    child->set_left(sibling);
    child->set_right(parent);
  }
  if (gparent) {
    if (parent == gparent->left) {
      gparent->set_left(child);
    } else {
      gparent->set_right(child);
    }
  } else {
    child->set_parent(nullptr);
  }
  child->subtree_data.bti_swap(parent);
}

template <class TNode>
constexpr void SwapAuto(TNode* node1, TNode* parent1, TNode* node2,
                        TNode* parent2) {
  assert(node1 && node2);
  if (node1 == node2) return;
  if (parent1 == node2) {
    SwapChildParent(node1, node2, parent2);
  } else if (parent2 == node1) {
    SwapChildParent(node2, node1, parent1);
  } else {
    SwapNotRelated(node1, parent1, node2, parent2);
  }
}
}  // namespace base
}  // namespace bst
