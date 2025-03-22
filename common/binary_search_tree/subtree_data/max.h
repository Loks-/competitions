#pragma once

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the maximum value in a BST subtree.
 *
 * This class extends another subtree data class to add maximum value tracking.
 * It maintains the maximum value among the node's data and the maximum values
 * of its left and right subtrees.
 *
 * @tparam MaxType The type used for the maximum value.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename MaxType, typename BaseData>
class Max : public BaseData {
 public:
  using Base = BaseData;
  using Self = Max<MaxType, BaseData>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  /**
   * @brief The maximum value in the subtree.
   */
  MaxType max_value;

 public:
  /**
   * @brief Updates the maximum value for a node.
   *
   * Updates the maximum value by comparing the node's data with the maximum
   * values from its left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    max_value = node->data;
    if (node->l)
      max_value = std::max(max_value, node->l->subtree_data.max_value);
    if (node->r)
      max_value = std::max(max_value, node->r->subtree_data.max_value);
  }
};

}  // namespace subtree_data
}  // namespace bst
