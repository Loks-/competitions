#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <utility>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains node ranks in a Weak AVL tree.
 *
 * This component stores the rank of each node without performing
 * any aggregation. The actual rank management is handled by the WAVL
 * tree implementation itself through the bti_* (Binary Tree Information)
 * functions.
 */
class WAVLRank : public Base {
 public:
  using Self = WAVLRank;

  /**
   * @brief Component capability flags.
   *
   * WAVLRank component only stores rank without aggregation.
   * Rank management is handled by the WAVL tree implementation
   * through bti_* functions during balancing operations.
   */
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the rank of a node.
   *
   * Helper function to access the rank of a node in the WAVL Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to get rank from.
   * @return The rank of the node.
   */
  template <typename TNode>
  static constexpr int get(const TNode* node) {
    assert(node);
    return node->subtree_data.template get<Self>().rank;
  }

  /**
   * @brief Sets the rank of a node.
   *
   * Helper function to set the rank of a node in the WAVL Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set rank for.
   * @param new_rank The new rank value to set.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, int new_rank) {
    assert(node);
    node->subtree_data.template get<Self>().rank = new_rank;
  }

  /**
   * @brief Increments the rank of a node by 1.
   *
   * Helper function to efficiently increase a node's rank.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to increment rank for.
   */
  template <typename TNode>
  static constexpr void inc(TNode* node) {
    assert(node);
    ++node->subtree_data.template get<Self>().rank;
  }

  /**
   * @brief Decrements the rank of a node by 1.
   *
   * Helper function to efficiently decrease a node's rank.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to decrement rank for.
   */
  template <typename TNode>
  static constexpr void dec(TNode* node) {
    assert(node);
    --node->subtree_data.template get<Self>().rank;
  }

  /**
   * @brief Resets the node rank to 0.
   *
   * In WAVL trees, new nodes are initialized with rank 0.
   * This function is called by the WAVL tree implementation during
   * node initialization and rebalancing operations.
   */
  constexpr void bti_reset() {
    rank = 0;  // New nodes have rank 0
  }

  /**
   * @brief Copies rank from another node.
   *
   * Used during WAVL tree balancing operations when
   * node ranks need to be copied.
   *
   * @tparam TNode The BST node type.
   * @param node The source node to copy rank from.
   */
  template <typename TNode>
  constexpr void bti_copy(const TNode* node) {
    assert(node);
    rank = get(node);
  }

  /**
   * @brief Swaps ranks between two nodes.
   *
   * Used during WAVL tree balancing operations when
   * node ranks need to be exchanged (e.g., during rotations).
   *
   * @tparam TNode The BST node type.
   * @param node The node to swap ranks with.
   */
  template <typename TNode>
  constexpr void bti_swap(TNode* node) {
    assert(node);
    std::swap(rank, node->subtree_data.template get<Self>().rank);
  }

 protected:
  /**
   * @brief The rank of the node in the Weak AVL tree.
   *
   * Used to maintain balance in WAVL trees.
   * By default, new nodes have rank 0.
   */
  int rank{0};
};

}  // namespace subtree_data
}  // namespace bst
