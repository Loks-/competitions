#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/base.h"
#include "common/template.h"

#include <utility>

namespace bst {
namespace deferred {

// Forward declaration of the helper function
template <typename TNode>
constexpr void reverse_subtree(TNode* node);

/**
 * @brief Deferred computation for reversing a subtree.
 *
 * This class manages the deferred reversal of a subtree in a BST. When a
 * subtree is reversed, the order of nodes in that subtree is inverted, which
 * affects both the tree structure and the order of nodes.
 *
 * The reversal operation is deferred to allow for batched processing of
 * multiple reversals, improving performance by reducing the number of tree
 * traversals.
 */
class Reverse : public Base {
 public:
  using Self = Reverse;

  /**
   * @brief Flag indicating if deferred operations modify tree structure.
   *
   * Reversing a subtree modifies the structure of the tree by changing
   * parent-child relationships.
   */
  static constexpr bool modify_tree_structure = true;

  /**
   * @brief Flag indicating if deferred operations modify node order.
   *
   * Reversing a subtree modifies the order of nodes within that subtree.
   */
  static constexpr bool modify_nodes_order = true;

  /**
   * @brief Resets the deferred computation state to empty.
   *
   * This function clears the reversed flag, indicating that there are
   * no pending deferred computations.
   */
  constexpr void clear() noexcept { reverse_required = false; }

  /**
   * @brief Checks if there are any deferred computations that need to be
   * applied.
   *
   * This function indicates whether there are any pending deferred computations
   * that need to be processed by calling apply(). Returns true if there are
   * computations waiting to be applied, false if all computations have been
   * processed.
   *
   * @return true if there are pending computations to apply, false otherwise.
   */
  constexpr bool apply_required() const noexcept { return reverse_required; }

  /**
   * @brief Marks the current subtree for reversal.
   *
   * This function indicates that the current subtree should be reversed.
   * The actual reversal will be deferred until apply() is called, and
   * even then, the reversal will be propagated to child nodes.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to be reversed.
   */
  template <typename TNode>
  constexpr void reverse_subtree(TNode*) noexcept {
    reverse_required = !reverse_required;
  }

  /**
   * @brief Applies pending subtree reversals.
   *
   * If reversal is required:
   * 1. Swaps left and right children of the current node
   * 2. Propagates the reversal request to child nodes
   * 3. Clears the reversal flag for the current node
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to apply reversals to.
   */
  template <typename TNode>
  constexpr void apply(TNode* node) noexcept {
    assert(node);
    if (reverse_required) {
      std::swap(node->left, node->right);
      bst::deferred::reverse_subtree(node->left);
      bst::deferred::reverse_subtree(node->right);
      reverse_required = false;
    }
  }

  /**
   * @brief Static interface for adding a reversal request.
   *
   * This is the standardized way to queue a reversal operation.
   * The value parameter type is Empty since reversal is just an action
   * trigger and doesn't need any additional data.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to reverse.
   * @param _ Ignored parameter, reversal is just an action trigger.
   */
  template <typename TNode>
  static constexpr void add(TNode* node, MetaEmpty) noexcept {
    bst::deferred::reverse_subtree(node);
  }

 protected:
  /**
   * @brief Flag indicating if the current subtree needs to be reversed.
   *
   * When true, indicates that the current subtree should be reversed.
   * Multiple calls to reverse_subtree() will toggle this flag, allowing
   * for cancellation of reversal operations.
   */
  bool reverse_required{false};
};

/**
 * @brief Helper function to mark a subtree for deferred reversal.
 *
 * This function provides a convenient way to request a subtree reversal
 * through the node's deferred computation manager. The actual reversal
 * will be performed when deferred computations are applied.
 *
 * @tparam TNode The BST node type.
 * @param node The root of the subtree to be reversed.
 */
template <typename TNode>
constexpr void reverse_subtree(TNode* node) {
  static_assert(TNode::DeferredType::template has<Reverse>(),
                "Node's Deferred class must contain Reverse class to use "
                "reverse_subtree");
  if (node) {
    node->deferred.reverse_subtree(node);
  }
}

}  // namespace deferred
}  // namespace bst
