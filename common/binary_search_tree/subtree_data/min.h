#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the minimum node value in a BST.
 *
 * This component tracks the minimum value in each subtree and supports
 * basic operations including insertion and segment operations.
 * The minimum is maintained efficiently through local updates during tree
 * modifications.
 *
 * @tparam MinType The type used for minimum values.
 */
template <typename MinType>
class Min : public Base {
 public:
  using Self = Min<MinType>;

  /**
   * @brief Component capability flags.
   *
   * Min component supports most operations and requires access to node data
   * to maintain the minimum value. Remove operations are not supported as
   * they would require recomputation of the minimum.
   */
  static constexpr bool use_data = true;
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;

  /**
   * @brief Gets the minimum from a SubtreeData instance.
   *
   * Helper function to access the minimum directly from a SubtreeData object.
   * Assumes that Min is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get minimum from.
   * @return The minimum stored in the subtree data.
   */
  template <typename TAggregators>
  static constexpr MinType get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) {
    return subtree_data.template get<Self>().value;
  }

  /**
   * @brief Gets the minimum value in a subtree.
   *
   * Helper function to access the minimum value in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to get minimum from.
   * @return The minimum value in the subtree.
   */
  template <typename Node>
  static constexpr MinType get(const Node* node) {
    assert(node);
    return get(node->subtree_data);
  }

  /**
   * @brief Sets the minimum value in a subtree.
   *
   * Helper function to set the minimum value in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to set minimum for.
   * @param value The new minimum value to set.
   */
  template <typename Node>
  static constexpr void set(Node* node, const MinType& value) {
    assert(node);
    node->subtree_data.template get<Self>().value = value;
  }

  /**
   * @brief Sets minimum to the node's value.
   *
   * For a single node, the minimum is equal to its value.
   *
   * @tparam Node The BST node type.
   * @param node The node to set minimum from.
   */
  template <typename Node>
  constexpr void set_node(const Node* node) {
    assert(node);
    value = MinType(node->data);
  }

  /**
   * @brief Copies minimum from another subtree.
   *
   * Used when replacing one subtree with another to maintain correct minimum.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to copy minimum from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    assert(node);
    value = get(node);
  }

  /**
   * @brief Updates minimum with node's value.
   *
   * Used in order-independent operations where a single node
   * is being added to the subtree.
   *
   * @tparam Node The BST node type.
   * @param node The node being added.
   */
  template <typename Node>
  constexpr void add_node(const Node* node) {
    assert(node);
    value = std::min(value, MinType(node->data));
  }

  /**
   * @brief Updates minimum with another subtree's minimum.
   *
   * Used in order-independent operations where an entire subtree
   * is being added to the current subtree.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree being added.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    assert(node);
    value = std::min(value, get(node));
  }

  /**
   * @brief Updates minimum with node's value during insertion.
   *
   * Similar to add_node but specifically used during tree
   * modification operations.
   *
   * @tparam Node The BST node type.
   * @param node The node being inserted.
   */
  template <typename Node>
  constexpr void insert_node(const Node* node) {
    assert(node);
    value = std::min(value, MinType(node->data));
  }

  /**
   * @brief Updates minimum with inserted subtree's minimum.
   *
   * Similar to add_subtree but specifically used during tree
   * modification operations.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree being inserted.
   */
  template <typename Node>
  constexpr void insert_subtree(const Node* node) {
    assert(node);
    value = std::min(value, get(node));
  }

 protected:
  /**
   * @brief The minimum value in the subtree.
   *
   * This value represents the minimum of all node values in the subtree,
   * including the root node.
   */
  MinType value{};
};

}  // namespace subtree_data
}  // namespace bst
