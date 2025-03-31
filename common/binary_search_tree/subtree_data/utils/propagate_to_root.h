#pragma once

#include "common/template.h"

namespace bst {
namespace subtree_data {

/**
 * @brief Updates subtree data from a node up to the root.
 *
 * This function ensures that all subtree data aggregators are updated along the
 * path from the given node to the root. It's used when node's data, keys,
 * structure, or node order has changed and we need to propagate these changes
 * upward.
 *
 * The updates are performed from the node up to the root to ensure that each
 * node's subtree data is updated with the latest changes from its children.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_to_root(TNode* node) {
  if constexpr (!TNode::SubtreeDataType::empty) {
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");

    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
  }
}

/**
 * @brief Updates only data-related subtree aggregators from node to root.
 *
 * Used when only the node's data has changed, not its structure, keys, or
 * node order. This is typically used for simple data updates that don't
 * affect the tree's structure or ordering.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_for_data_update(TNode* node) {
  if constexpr (TNode::SubtreeDataType::use_data) {
    propagate_to_root(node);
  }
}

/**
 * @brief Updates only key-related subtree aggregators from node to root.
 *
 * Used when only the node's keys have changed, not its data, structure, or
 * node order. This is typically used for key updates that don't affect
 * the tree's structure or ordering.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_for_key_update(TNode* node) {
  if constexpr (TNode::SubtreeDataType::use_keys) {
    propagate_to_root(node);
  }
}

/**
 * @brief Updates only tree structure-related subtree aggregators from node to
 * root.
 *
 * Used when only the tree structure has changed (e.g., rotations, rebalancing),
 * not its data, keys, or node order. This is typically used for structural
 * changes that don't affect the data or ordering of nodes.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_for_structure_update(TNode* node) {
  if constexpr (TNode::SubtreeDataType::use_tree_structure) {
    propagate_to_root(node);
  }
}

/**
 * @brief Updates only nodes order-related subtree aggregators from node to
 * root.
 *
 * Used when only the order of nodes in the subtree has changed, not its
 * data, structure, or keys. This is typically used for operations that
 * reorder nodes but don't change their data or the tree's structure.
 *
 * @tparam TNode The BST node type.
 * @param node The starting node to update from.
 */
template <typename TNode>
constexpr void propagate_for_nodes_order_update(TNode* node) {
  if constexpr (TNode::SubtreeDataType::use_nodes_order) {
    propagate_to_root(node);
  }
}
}  // namespace subtree_data
}  // namespace bst
