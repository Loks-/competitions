#pragma once

#include "common/binary_search_tree/subtree_data/base.h"
#include "common/hash.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains pseudo-random heights for nodes in a Treap.
 *
 * A Treap (Tree + Heap) is a randomized binary search tree that maintains
 * both BST properties for keys and heap properties for randomly assigned
 * heights. This component generates and stores these random heights, which are
 * used to maintain probabilistic balance in the tree.
 */
class TreapHeight : public Base {
 public:
  using Self = TreapHeight;

  /**
   * @brief Component capability flags.
   *
   * TreapHeight component only generates and stores random heights.
   * The actual balancing using these heights is handled by the Treap
   * implementation. Since heights are assigned once at node creation
   * and never modified, this component supports all operations without
   * needing special handling during modifications.
   */
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the pseudo-random height of a node.
   *
   * Helper function to access the height of a node in the Treap.
   * Assumes the node exists.
   *
   * @tparam Node The BST node type.
   * @param node The node to get height from.
   * @return The pseudo-random height of the node.
   */
  template <typename Node>
  static constexpr unsigned get(const Node* node) {
    assert(node);
    return node->subtree_data.template get<Self>().treap_height;
  }

  /**
   * @brief Sets the pseudo-random height of a node.
   *
   * Helper function to set the height of a node in the Treap.
   * Assumes the node exists. Note that this should be used with caution
   * as heights are typically only set during node creation.
   *
   * @tparam Node The BST node type.
   * @param node The node to set height for.
   * @param height The new height value to set.
   */
  template <typename Node>
  static constexpr void set(Node* node, unsigned height) {
    assert(node);
    node->subtree_data.template get<Self>().treap_height = height;
  }

  /**
   * @brief Initializes a node's height using a deterministic hash of its
   * raw_index.
   *
   * This is the only function needed for Treap balancing as heights are:
   * 1. Assigned once during node creation
   * 2. Never modified during the node's lifetime
   * 3. Deterministically generated from raw_index for reproducibility
   *
   * The nhash::PolicySafe ensures a good distribution of heights while
   * maintaining deterministic behavior.
   *
   * @param index The unique index of the node, used as seed for height
   * generation.
   */
  constexpr void initialize(unsigned index) {
    Base::initialize(index);
    treap_height = nhash::HValue<nhash::PolicySafe>(index);
  }

 protected:
  /**
   * @brief The pseudo-random height of the node in the Treap.
   *
   * This value is generated deterministically from the node's raw_index
   * during creation and remains unchanged throughout the node's lifetime.
   * The Treap uses this height to maintain heap property: a node's height
   * should be greater than its children's heights.
   */
  unsigned treap_height{0};
};

}  // namespace subtree_data
}  // namespace bst
