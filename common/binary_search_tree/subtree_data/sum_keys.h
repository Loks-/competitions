#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief A class that maintains the sum of keys in a BST subtree.
 *
 * This class extends another subtree data class to add key sum functionality.
 * It maintains the sum of the node's key and the sums of keys from its left
 * and right subtrees.
 *
 * @tparam SumType The type used for the sum calculation.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename SumType, typename BaseData>
class SumKeys : public BaseData {
 public:
  using Base = BaseData;
  using Self = SumKeys<SumType, BaseData>;

  static constexpr bool is_none = false;
  static constexpr bool use_keys = true;

 public:
  /**
   * @brief The sum of all keys in the subtree.
   *
   * Includes the node's key and the sums from both left and right subtrees.
   */
  SumType sum_keys;

 public:
  /**
   * @brief Updates the sum of keys in the subtree.
   *
   * Calculates the total sum as the node's key plus the sums from
   * the left and right subtrees.
   *
   * @param node The node to update.
   */
  template <typename Node>
  constexpr void update(Node* node) {
    Base::update(node);
    sum_keys = node->key;
    if (node->l) sum_keys += node->l->subtree_data.sum_keys;
    if (node->r) sum_keys += node->r->subtree_data.sum_keys;
  }
};

}  // namespace subtree_data
}  // namespace bst
