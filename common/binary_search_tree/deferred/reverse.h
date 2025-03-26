#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/base.h"
#include "common/template.h"

#include <utility>

namespace bst {
namespace deferred {

// Forward declaration of the helper function
template <class Node>
constexpr void reverse_subtree(Node* node);

/**
 * @brief A deferred computation manager for reversing subtrees in a BST.
 *
 * This class manages deferred subtree reversal operations in a binary search
 * tree. It allows multiple reversal operations to be queued and applied
 * efficiently by deferring actual reversals to child nodes until necessary.
 *
 * The class maintains a flag indicating if the current subtree needs to be
 * reversed. When applying reversals, it swaps children of the current node
 * and propagates the reversal request to child nodes. This lazy approach
 * allows for efficient handling of multiple reversal operations.
 */
class Reverse : public Base {
 public:
  using Self = Reverse;

  /**
   * @brief Flag indicating if deferred operations modify tree structure.
   *
   * Set to true because reversing a subtree modifies the tree structure
   * by swapping left and right children.
   */
  static constexpr bool modify_tree = true;

  /**
   * @brief Resets the deferred computation state to empty.
   *
   * Clears the reversal flag.
   */
  constexpr void clear() { reverse_required = false; }

  /**
   * @brief Checks if there are any deferred computations that need to be
   * applied.
   *
   * @return true if the current subtree needs to be reversed, false otherwise.
   */
  constexpr bool apply_required() const { return reverse_required; }

  /**
   * @brief Marks the current subtree for reversal.
   *
   * This function indicates that the current subtree should be reversed.
   * The actual reversal will be deferred until apply() is called, and
   * even then, the reversal will be propagated to child nodes.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to be reversed.
   */
  template <class Node>
  constexpr void reverse_subtree(Node*) {
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
   * @tparam Node The BST node type.
   * @param node The root of the subtree to apply reversals to.
   */
  template <class Node>
  constexpr void apply(Node* node) {
    assert(node);
    if (reverse_required) {
      std::swap(node->l, node->r);
      bst::deferred::reverse_subtree(node->l);
      bst::deferred::reverse_subtree(node->r);
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
   * @tparam Node The BST node type.
   * @param node The root of the subtree to reverse.
   * @param _ Ignored parameter, reversal is just an action trigger.
   */
  template <class Node>
  static constexpr void add(Node* node, MetaEmpty) {
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
 * @tparam Node The BST node type.
 * @param node The root of the subtree to be reversed.
 */
template <class Node>
constexpr void reverse_subtree(Node* node) {
  static_assert(Node::TAction::template has<Reverse>(),
                "Node's Deferred class must contain Reverse class to use "
                "reverse_subtree");
  if (node) {
    node->deferred.reverse_subtree(node);
  }
}

}  // namespace deferred
}  // namespace bst
