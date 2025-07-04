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
 * The function handles different types of modifications:
 * - Tree structure changes (e.g., rotations)
 * - Node order changes (e.g., reversals)
 *
 * @tparam TNode The BST node type.
 * @param node The target node to apply computations to.
 */
template <typename TNode>
inline void propagate_to_node(TNode* node) {
  if constexpr (!TNode::DeferredType::empty) {
    static_assert(TNode::has_parent, "Node must have parent pointer enabled");
    thread_local std::stack<TNode*> path;

    // Build path from root to node
    for (; node; node = node->parent) path.push(node);

    // Apply computations from root to target node
    for (; !path.empty(); path.pop()) path.top()->apply_deferred();
  }
}

}  // namespace deferred
}  // namespace bst
