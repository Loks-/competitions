#pragma once

#include "common/base.h"

#include <utility>

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the color of a node in a Red-Black Tree.
 *
 * This class extends another subtree data class to add color tracking
 * for Red-Black Tree operations. A node can be either red (black = false)
 * or black (black = true).
 *
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename BaseData>
class RBTColor : public BaseData {
 public:
  using Base = BaseData;
  using Self = RBTColor<BaseData>;

 public:
  /**
   * @brief The color of the node.
   *
   * true for black, false for red.
   * Initialized to false (red).
   */
  bool black = false;

 public:
  /**
   * @brief Resets the Red-Black Tree information.
   *
   * Sets the node color to red (black = false).
   */
  constexpr void bti_reset() {
    Base::bti_reset();
    black = false;
  }

  /**
   * @brief Copies Red-Black Tree information from another node.
   *
   * @param node The source node to copy from.
   */
  template <typename Node>
  constexpr void bti_copy(const Node* node) {
    assert(node);
    Base::bti_copy(node);
    black = node->subtree_data.black;
  }

  /**
   * @brief Swaps Red-Black Tree information with another node.
   *
   * @param node The node to swap with.
   */
  template <typename Node>
  constexpr void bti_swap(Node* node) {
    assert(node);
    Base::bti_swap(node);
    std::swap(black, node->subtree_data.black);
  }
};

}  // namespace subtree_data
}  // namespace bst
