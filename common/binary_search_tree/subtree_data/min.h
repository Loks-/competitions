#pragma once

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the minimum value in a BST subtree.
 *
 * This class extends another subtree data class to add minimum value tracking.
 * It maintains the minimum value among the node's data and the minimum values
 * of its left and right subtrees.
 *
 * @tparam MinType The type used for the minimum value.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename MinType, typename BaseData>
class Min : public BaseData {
 public:
  using Base = BaseData;
  using Self = Min<MinType, BaseData>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  /**
   * @brief The minimum value in the subtree.
   */
  MinType min_value;

 public:
  /**
   * @brief Updates the minimum value for a node.
   *
   * Updates the minimum value by comparing the node's data with the minimum
   * values from its left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    min_value = node->data;
    if (node->l)
      min_value = std::min(min_value, node->l->subtree_data.min_value);
    if (node->r)
      min_value = std::min(min_value, node->r->subtree_data.min_value);
  }
};

}  // namespace subtree_data
}  // namespace bst
