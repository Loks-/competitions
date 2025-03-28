#pragma once

#include "common/template.h"

#include <stack>

namespace bst {
namespace deferred {

/**
 * @brief Propagates deferred computations from root to a specific node.
 *
 * This function ensures that all deferred computations are applied to the path
 * from root to the given node. It's used when we need to access the current
 * state of a node, ensuring all pending changes are applied.
 *
 * @tparam Node The BST node type.
 * @param node The target node to apply computations to.
 */
template <class Node>
inline void propagate_to_node(Node* node) {
  if constexpr (!Node::DeferredType::empty) {
    static_assert(Node::has_parent, "Node must have parent pointer enabled");
    thread_local std::stack<Node*> path;

    // Build path from root to node
    for (; node; node = node->parent) {
      path.push(node);
    }

    // Apply computations from root to target node
    while (!path.empty()) {
      path.top()->apply_deferred();
      path.pop();
    }
  }
}

/**
 * @brief Propagates only data-modifying computations from root to node.
 *
 * Used when only the node's data needs to be accessed, not its structure or
 * keys.
 *
 * @tparam Node The BST node type.
 * @param node The target node to apply computations to.
 */
template <class Node>
inline void propagate_for_data_access(Node* node) {
  if constexpr (Node::DeferredType::modify_data) {
    propagate_to_node(node);
  }
}

/**
 * @brief Propagates only key-modifying computations from root to node.
 *
 * Used when only the node's keys need to be accessed, not its data or
 * structure.
 *
 * @tparam Node The BST node type.
 * @param node The target node to apply computations to.
 */
template <class Node>
inline void propagate_for_key_access(Node* node) {
  if constexpr (Node::DeferredType::modify_keys) {
    propagate_to_node(node);
  }
}

/**
 * @brief Propagates only tree structure-modifying computations from root to
 * node.
 *
 * Used when only the tree structure needs to be accessed, not its data or keys.
 *
 * @tparam Node The BST node type.
 * @param node The target node to apply computations to.
 */
template <class Node>
inline void propagate_for_structure_access(Node* node) {
  if constexpr (Node::DeferredType::modify_tree) {
    propagate_to_node(node);
  }
}

}  // namespace deferred
}  // namespace bst
