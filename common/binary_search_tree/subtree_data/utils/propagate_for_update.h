#pragma once

namespace bst {
namespace subtree_data {

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
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");
    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
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
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");
    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
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
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");
    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
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
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");
    // Update subtree data from node up to root
    for (; node; node = node->parent) node->update_subtree_data();
  }
}

}  // namespace subtree_data
}  // namespace bst
