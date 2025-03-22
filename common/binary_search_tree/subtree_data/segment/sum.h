#pragma once

#include "common/binary_search_tree/subtree_data/sum.h"

namespace bst {
namespace subtree_data {
namespace segment {

/**
 * @brief A class that maintains the sum of values in a BST segment.
 *
 * This class extends the base Sum class to add segment-specific functionality.
 * It provides methods to set and add sums for both individual nodes
 * and entire subtrees.
 *
 * @tparam SumType The type used for the sum value.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename SumType, typename BaseData>
class Sum : public bst::subtree_data::Sum<SumType, BaseData> {
 public:
  using Base = bst::subtree_data::Sum<SumType, BaseData>;
  using Self = Sum<SumType, BaseData>;

 public:
  /**
   * @brief Constructs a new Sum segment with zero value.
   */
  constexpr Sum() { Base::sum_value = SumType(0); }

  /**
   * @brief Sets the sum for a single node.
   *
   * Sets the sum to the node's data.
   *
   * @param node The node to set sum from.
   */
  template <typename Node>
  constexpr void set_node(const Node* node) {
    Base::set_node(node);
    Base::sum_value = node->data;
  }

  /**
   * @brief Sets the sum for a subtree.
   *
   * Sets the sum to match the sum of the given subtree.
   *
   * @param node The root of the subtree to get sum from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    Base::set_subtree(node);
    Base::sum_value = node->subtree_data.sum_value;
  }

  /**
   * @brief Adds the sum of a single node.
   *
   * Updates the sum by adding the node's data if the node exists.
   *
   * @param node The node to add sum from.
   */
  template <typename Node>
  constexpr void add_node(const Node* node) {
    Base::add_node(node);
    if (node) {
      Base::sum_value += node->data;
    }
  }

  /**
   * @brief Adds the sum of a subtree.
   *
   * Updates the sum by adding the subtree's sum value
   * if the node exists.
   *
   * @param node The root of the subtree to add sum from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    Base::add_subtree(node);
    if (node) {
      Base::sum_value += node->subtree_data.sum_value;
    }
  }
};

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
