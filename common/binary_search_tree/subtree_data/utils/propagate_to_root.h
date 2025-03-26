#pragma once

#include "common/template.h"

namespace bst {
namespace subtree_data {

/**
 * @brief Updates subtree data from a node up to the root.
 *
 * This function ensures that all subtree data aggregators are updated along the
 * path from the given node to the root. It's used when node's data or its
 * subtree structure has changed and we need to propagate these changes upward.
 *
 * The updates are performed from the node up to the root to ensure that each
 * node's subtree data is updated with the latest changes from its children.
 *
 * @tparam Node The BST node type.
 * @param node The starting node to update from.
 */
template <class Node>
inline void propagate_to_root(Node* node) {
  if constexpr (!Node::TInfo::empty) {
    static_assert(Node::use_parent, "Node must have parent pointer enabled");

    // Update subtree data from node up to root
    for (; node; node = node->p) {
      node->UpdateInfo();
    }
  }
}

/**
 * @brief Updates only data-related subtree aggregators from node to root.
 *
 * Used when only the node's data has changed, not its structure or keys.
 *
 * @tparam Node The BST node type.
 * @param node The starting node to update from.
 */
template <class Node>
inline void propagate_for_data_update(Node* node) {
  if constexpr (Node::TInfo::modify_data) {
    propagate_to_root(node);
  }
}

/**
 * @brief Updates only key-related subtree aggregators from node to root.
 *
 * Used when only the node's keys have changed, not its data or structure.
 *
 * @tparam Node The BST node type.
 * @param node The starting node to update from.
 */
template <class Node>
inline void propagate_for_key_update(Node* node) {
  if constexpr (Node::TInfo::modify_keys) {
    propagate_to_root(node);
  }
}

/**
 * @brief Updates only tree structure-related subtree aggregators from node to
 * root.
 *
 * Used when only the tree structure has changed, not its data or keys.
 *
 * @tparam Node The BST node type.
 * @param node The starting node to update from.
 */
template <class Node>
inline void propagate_for_structure_update(Node* node) {
  if constexpr (Node::TInfo::modify_tree) {
    propagate_to_root(node);
  }
}

}  // namespace subtree_data
}  // namespace bst
