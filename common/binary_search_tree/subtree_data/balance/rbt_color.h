#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <utility>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains node colors in a Red-Black Tree.
 *
 * This component tracks whether each node is black or red, which is used
 * to maintain the Red-Black Tree invariants. The color is stored as a boolean
 * where true represents black and false represents red.
 */
class RBTColor : public Base {
 public:
  using Self = RBTColor;

  /**
   * @brief Component capability flags.
   *
   * RBTColor component only stores color without aggregation.
   * Color management is handled by the Red-Black Tree implementation
   * through bti_* functions during balancing operations.
   */
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets whether a node is black.
   *
   * Helper function to check if a node is black in the Red-Black Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to check color for.
   * @return True if the node is black, false if red.
   */
  template <typename TNode>
  static constexpr bool get(const TNode* node) {
    assert(node);
    return node->subtree_data.template get<Self>().is_black;
  }

  /**
   * @brief Sets whether a node is black.
   *
   * Helper function to set the color of a node in the Red-Black Tree.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set color for.
   * @param black True to set the node black, false to set it red.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, bool black) {
    assert(node);
    node->subtree_data.template get<Self>().is_black = black;
  }

  /**
   * @brief Resets the node color to red.
   *
   * In Red-Black trees, new nodes are initialized as red.
   * This function is called by the Red-Black tree implementation during
   * node initialization and rebalancing operations.
   */
  constexpr void bti_reset() noexcept {
    is_black = false;  // New nodes are red
  }

  /**
   * @brief Copies color from another node.
   *
   * Used during Red-Black tree balancing operations when
   * node colors need to be copied.
   *
   * @tparam TNode The BST node type.
   * @param node The source node to copy color from.
   */
  template <typename TNode>
  constexpr void bti_copy(const TNode* node) noexcept {
    assert(node);
    is_black = get(node);
  }

  /**
   * @brief Swaps colors between two nodes.
   *
   * Used during Red-Black tree balancing operations when
   * node colors need to be exchanged (e.g., during rotations).
   *
   * @tparam TNode The BST node type.
   * @param node The node to swap colors with.
   */
  template <typename TNode>
  constexpr void bti_swap(TNode* node) noexcept {
    assert(node);
    std::swap(is_black, node->subtree_data.template get<Self>().is_black);
  }

 protected:
  /**
   * @brief The color of the node in the Red-Black Tree.
   *
   * True represents black, false represents red.
   * By default, new nodes are red (is_black = false).
   */
  bool is_black{false};
};

}  // namespace subtree_data
}  // namespace bst
