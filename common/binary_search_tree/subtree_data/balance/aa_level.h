#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <utility>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains node levels in an AA tree.
 *
 * This component stores the level of each node without performing
 * any aggregation. The actual level management is handled by the AA
 * tree implementation itself through the bti_* (Binary Tree Information)
 * functions.
 */
class AALevel : public Base {
 public:
  using Self = AALevel;

  /**
   * @brief Component capability flags.
   *
   * AALevel component only stores level without aggregation.
   * Level management is handled by the AA tree implementation
   * through bti_* functions during balancing operations.
   */
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the level of a node.
   *
   * Helper function to access the level of a node in the AA Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to get rank from.
   * @return The level of the node.
   */
  template <typename TNode>
  static constexpr unsigned get(const TNode* node) noexcept {
    assert(node);
    return node->subtree_data.template get<Self>().level;
  }

  /**
   * @brief Sets the level of a node.
   *
   * Helper function to set the level of a node in the AA Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set level for.
   * @param new_level The new level value to set.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, unsigned new_level) noexcept {
    assert(node);
    node->subtree_data.template get<Self>().level = new_level;
  }

  /**
   * @brief Increments the level of a node by 1.
   *
   * Helper function to efficiently increase a node's level.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to increment level for.
   */
  template <typename TNode>
  static constexpr void inc(TNode* node) noexcept {
    assert(node);
    ++node->subtree_data.template get<Self>().level;
  }

  /**
   * @brief Resets the node level to 1.
   *
   * In AA trees, new nodes are initialized with level 1.
   * This function is called by the AA tree implementation during
   * node initialization and rebalancing operations.
   */
  constexpr void bti_reset() noexcept {
    level = 1;  // New nodes have level 1
  }

  /**
   * @brief Copies level from another node.
   *
   * Used during AA tree balancing operations when
   * node levels need to be copied.
   *
   * @tparam TNode The BST node type.
   * @param node The source node to copy level from.
   */
  template <typename TNode>
  constexpr void bti_copy(const TNode* node) noexcept {
    assert(node);
    level = get(node);
  }

  /**
   * @brief Swaps levels between two nodes.
   *
   * Used during AA tree balancing operations when
   * node levels need to be exchanged (e.g., during rotations).
   *
   * @tparam TNode The BST node type.
   * @param node The node to swap levels with.
   */
  template <typename TNode>
  constexpr void bti_swap(TNode* node) noexcept {
    assert(node);
    std::swap(level, node->subtree_data.template get<Self>().level);
  }

 protected:
  /**
   * @brief The level of the node in the AA tree.
   *
   * Used to maintain balance in AA trees.
   * By default, new nodes have level 1.
   */
  unsigned level{1};  // New nodes have level 1
};

}  // namespace subtree_data
}  // namespace bst
