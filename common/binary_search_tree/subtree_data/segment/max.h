#pragma once

#include "common/binary_search_tree/subtree_data/max.h"

#include <algorithm>
#include <limits>

namespace bst {
namespace subtree_data {
namespace segment {

/**
 * @brief A class that maintains the maximum value in a BST segment.
 *
 * This class extends the base Max class to add segment-specific functionality.
 * It provides methods to set and add maximum values for both individual nodes
 * and entire subtrees.
 *
 * @tparam MaxType The type used for the maximum value.
 * @tparam BaseData The base subtree data class to extend.
 */
template <typename MaxType, typename BaseData>
class Max : public bst::subtree_data::Max<MaxType, BaseData> {
 public:
  using Base = bst::subtree_data::Max<MaxType, BaseData>;
  using Self = Max<MaxType, BaseData>;

 public:
  /**
   * @brief Constructs a new Max segment with minimum possible value.
   */
  constexpr Max() { Base::max_value = std::numeric_limits<MaxType>::min(); }

  /**
   * @brief Sets the maximum value for a single node.
   *
   * Sets the maximum value to the node's data.
   *
   * @param node The node to set maximum value from.
   */
  template <typename Node>
  constexpr void set_node(const Node* node) {
    Base::set_node(node);
    Base::max_value = node->data;
  }

  /**
   * @brief Sets the maximum value for a subtree.
   *
   * Sets the maximum value to match the maximum value of the given subtree.
   *
   * @param node The root of the subtree to get maximum value from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    Base::set_subtree(node);
    Base::max_value = node->subtree_data.max_value;
  }

  /**
   * @brief Adds the maximum value of a single node.
   *
   * Updates the maximum value by comparing with the node's data if the node
   * exists.
   *
   * @param node The node to add maximum value from.
   */
  template <typename Node>
  constexpr void add_node(const Node* node) {
    Base::add_node(node);
    if (node) {
      Base::max_value = std::max(Base::max_value, node->data);
    }
  }

  /**
   * @brief Adds the maximum value of a subtree.
   *
   * Updates the maximum value by comparing with the subtree's maximum value
   * if the node exists.
   *
   * @param node The root of the subtree to add maximum value from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    Base::add_subtree(node);
    if (node) {
      Base::max_value = std::max(Base::max_value, node->subtree_data.max_value);
    }
  }
};

}  // namespace segment
}  // namespace subtree_data
}  // namespace bst
