#pragma once

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the height of a BST subtree.
 *
 * This class extends another subtree data class to add height tracking.
 * The height of a leaf node is 1, and the height of an internal node
 * is 1 plus the maximum height of its children.
 *
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename BaseData>
class Height : public BaseData {
 public:
  using Base = BaseData;
  using Self = Height<BaseData>;

  static constexpr bool is_none = false;

 public:
  /**
   * @brief The height of the subtree.
   *
   * For a leaf node, height = 1.
   * For an internal node, height = 1 + max(left_height, right_height).
   */
  unsigned height;

 public:
  /**
   * @brief Updates the height of the subtree.
   *
   * Calculates the height as 1 plus the maximum height of the left
   * and right subtrees. A leaf node has height 1.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    height = 1 + std::max((node->l ? node->l->subtree_data.height : 0),
                          (node->r ? node->r->subtree_data.height : 0));
  }
};

}  // namespace subtree_data
}  // namespace bst
