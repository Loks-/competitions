#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the maximum node value in a BST.
 *
 * This component tracks the maximum value in each subtree and supports
 * basic operations including insertion and segment operations.
 * The maximum is maintained efficiently through local updates during tree
 * modifications.
 *
 * @tparam MaxType The type used for maximum values.
 */
template <typename MaxType>
class Max : public Base {
 public:
  using Self = Max<MaxType>;

  /**
   * @brief Component capability flags.
   *
   * Max component supports most operations and requires access to node data
   * to maintain the maximum value. Remove operations are not supported as
   * they would require recomputation of the maximum.
   */
  static constexpr bool use_data = true;
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;

  /**
   * @brief Gets the maximum from a SubtreeData instance.
   *
   * Helper function to access the maximum directly from a SubtreeData object.
   * Assumes that Max is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get maximum from.
   * @return The maximum stored in the subtree data.
   */
  template <typename TAggregators>
  static constexpr MaxType get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) {
    return subtree_data.template get<Self>().value;
  }

  /**
   * @brief Gets the maximum value in a subtree.
   *
   * Helper function to access the maximum value in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to get maximum from.
   * @return The maximum value in the subtree.
   */
  template <typename TNode>
  static constexpr MaxType get(const TNode* node) {
    assert(node);
    return get(node->subtree_data);
  }

  /**
   * @brief Sets the maximum value in a subtree.
   *
   * Helper function to set the maximum value in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to set maximum for.
   * @param value The new maximum value to set.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, const MaxType& value) {
    assert(node);
    node->subtree_data.template get<Self>().value = value;
  }

  /**
   * @brief Sets maximum to the node's value.
   *
   * For a single node, the maximum is equal to its value.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set maximum from.
   */
  template <typename TNode>
  constexpr void set_node(const TNode* node) {
    assert(node);
    value = MaxType(node->data);
  }

  /**
   * @brief Copies maximum from another subtree.
   *
   * Used when replacing one subtree with another to maintain correct maximum.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to copy maximum from.
   */
  template <typename TNode>
  constexpr void set_subtree(const TNode* node) {
    assert(node);
    value = get(node);
  }

  /**
   * @brief Updates maximum with node's value.
   *
   * Used in order-independent operations where a single node
   * is being added to the subtree.
   *
   * @tparam TNode The BST node type.
   * @param node The node being added.
   */
  template <typename TNode>
  constexpr void add_node(const TNode* node) {
    assert(node);
    value = std::max(value, MaxType(node->data));
  }

  /**
   * @brief Updates maximum with another subtree's maximum.
   *
   * Used in order-independent operations where an entire subtree
   * is being added to the current subtree.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree being added.
   */
  template <typename TNode>
  constexpr void add_subtree(const TNode* node) {
    assert(node);
    value = std::max(value, get(node));
  }

  /**
   * @brief Updates maximum with node's value during insertion.
   *
   * Similar to add_node but specifically used during tree
   * modification operations.
   *
   * @tparam TNode The BST node type.
   * @param node The node being inserted.
   */
  template <typename TNode>
  constexpr void insert_node(const TNode* node) {
    assert(node);
    value = std::max(value, MaxType(node->data));
  }

  /**
   * @brief Updates maximum with inserted subtree's maximum.
   *
   * Similar to add_subtree but specifically used during tree
   * modification operations.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree being inserted.
   */
  template <typename TNode>
  constexpr void insert_subtree(const TNode* node) {
    assert(node);
    value = std::max(value, get(node));
  }

 protected:
  /**
   * @brief The maximum value in the subtree.
   *
   * This value represents the maximum of all node values in the subtree,
   * including the root node.
   */
  MaxType value{};
};

}  // namespace subtree_data
}  // namespace bst
