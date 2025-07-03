#pragma once

#include "common/base.h"

#include <utility>

namespace bst {
namespace base {

/**
 * @brief Swaps two unrelated nodes in a binary search tree.
 *
 * This function swaps the positions of two nodes that are not directly related
 * (neither is a parent of the other). The function handles all the necessary
 * updates to maintain the tree structure:
 * - Swaps the children of both nodes
 * - Updates parent-child relationships
 * - Handles cases where both nodes share the same parent
 * - Swaps subtree data between the nodes
 *
 * Note: This function does not apply deferred computations or update subtree
 * data, as it assumes the caller will handle these operations as needed.
 *
 * @tparam TNode The BST node type
 * @param node1 The first node to swap
 * @param parent1 The parent of the first node
 * @param node2 The second node to swap
 * @param parent2 The parent of the second node
 */
template <class TNode>
constexpr void swap_unrelated(TNode* node1, TNode* parent1, TNode* node2,
                              TNode* parent2) {
  assert(node1 != node2);

  // Store the children of both nodes
  TNode *child1l = node1->left, *child1r = node1->right;

  // Swap the children
  node1->set_left(node2->left);
  node1->set_right(node2->right);
  node2->set_left(child1l);
  node2->set_right(child1r);

  // Handle the case where both nodes share the same parent
  if (parent1 && parent1 == parent2) {
    std::swap(parent1->left, parent1->right);
    return;
  }

  // Update parent1's child pointer
  if (parent1) {
    if (node1 == parent1->left)
      parent1->set_left(node2);
    else
      parent1->set_right(node2);
  } else {
    node2->set_parent(nullptr);
  }

  // Update parent2's child pointer
  if (parent2) {
    if (node2 == parent2->left)
      parent2->set_left(node1);
    else
      parent2->set_right(node1);
  } else {
    node1->set_parent(nullptr);
  }

  // Swap subtree data between the nodes
  node1->subtree_data.bti_swap(node2);
}

/**
 * @brief Swaps a child node with its parent in a binary search tree.
 *
 * This function performs a swap operation between a child node and its parent,
 * effectively promoting the child to the parent's position. The function:
 * - Reorganizes the tree structure to maintain the BST property
 * - Updates all parent-child relationships
 * - Handles the grandparent relationship
 * - Swaps subtree data between the nodes
 *
 * This operation is commonly used in tree balancing algorithms and splay
 * operations where a node needs to be moved up in the tree.
 *
 * @tparam TNode The BST node type
 * @param child The child node to swap with its parent
 * @param parent The parent node to swap with its child
 * @param grandparent The grandparent node (parent of the parent)
 */
template <class TNode>
constexpr void swap_child_parent(TNode* child, TNode* parent,
                                 TNode* grandparent) {
  // Store the sibling of the child
  TNode* sibling;

  if (child == parent->left) {
    sibling = parent->right;
    parent->set_left(child->left);
    parent->set_right(child->right);
    child->set_left(parent);
    child->set_right(sibling);
  } else {
    sibling = parent->left;
    parent->set_left(child->left);
    parent->set_right(child->right);
    child->set_left(sibling);
    child->set_right(parent);
  }

  // Update grandparent's child pointer
  if (grandparent) {
    if (parent == grandparent->left) {
      grandparent->set_left(child);
    } else {
      grandparent->set_right(child);
    }
  } else {
    child->set_parent(nullptr);
  }

  // Swap subtree data between the nodes
  child->subtree_data.bti_swap(parent);
}

/**
 * @brief Automatically determines the appropriate swap operation and performs
 * it.
 *
 * This function is a convenience wrapper that automatically detects the
 * relationship between two nodes and performs the appropriate swap operation:
 * - If one node is the parent of the other, it calls swap_child_parent
 * - If the nodes are unrelated, it calls swap_unrelated
 * - If the nodes are the same, no operation is performed
 *
 * This function is useful when the relationship between nodes is not known
 * in advance, such as in generic tree manipulation algorithms.
 *
 * @tparam TNode The BST node type
 * @param node1 The first node to swap
 * @param parent1 The parent of the first node
 * @param node2 The second node to swap
 * @param parent2 The parent of the second node
 */
template <class TNode>
constexpr void swap_nodes(TNode* node1, TNode* parent1, TNode* node2,
                          TNode* parent2) {
  assert(node1 && node2);
  assert(!node1->apply_required() && !node2->apply_required());

  // If nodes are the same, no swap needed
  if (node1 == node2) return;

  // Determine the relationship and perform appropriate swap
  if (parent1 == node2) {
    // node2 is the parent of node1
    swap_child_parent(node1, node2, parent2);
  } else if (parent2 == node1) {
    // node1 is the parent of node2
    swap_child_parent(node2, node1, parent1);
  } else {
    // Nodes are unrelated
    swap_unrelated(node1, parent1, node2, parent2);
  }
}

}  // namespace base
}  // namespace bst
