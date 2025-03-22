#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the sum of values in a BST subtree.
 *
 * This class extends another subtree data class to add sum functionality.
 * It maintains the sum of the node's data and the sums of its left and right
 * subtrees.
 *
 * @tparam SumType The type used for the sum calculation.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename SumType, typename BaseData>
class Sum : public BaseData {
 public:
  using Base = BaseData;
  using Self = Sum<SumType, BaseData>;

  static constexpr bool is_none = false;
  static constexpr bool use_data = true;

 public:
  /**
   * @brief The sum of all values in the subtree.
   *
   * Includes the node's data and the sums from both left and right subtrees.
   */
  SumType sum_value;

 public:
  /**
   * @brief Updates the sum of values in the subtree.
   *
   * Calculates the total sum as the node's data plus the sums from
   * the left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    sum_value = node->data;
    if (node->l) sum_value += node->l->subtree_data.sum_value;
    if (node->r) sum_value += node->r->subtree_data.sum_value;
  }
};

}  // namespace subtree_data
}  // namespace bst
