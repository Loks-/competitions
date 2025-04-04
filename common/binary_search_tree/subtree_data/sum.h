#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the sum of node values in a BST.
 *
 * This component tracks the sum of values in each subtree and supports
 * all basic operations including insertion, removal, and segment operations.
 * The sum is maintained efficiently through local updates during tree
 * modifications.
 *
 * @tparam SumType The type used for sum values.
 */
template <typename SumType>
class Sum : public Base {
 public:
  using Self = Sum<SumType>;

  /**
   * @brief Component capability flags.
   *
   * Sum component supports all operations and requires access to node data
   * to maintain the sum of values.
   */
  static constexpr bool use_data = true;
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the sum from a SubtreeData instance.
   *
   * Helper function to access the sum directly from a SubtreeData object.
   * Assumes that Sum is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get sum from.
   * @return The sum stored in the subtree data.
   */
  template <typename TAggregators>
  static constexpr const SumType& get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) noexcept {
    return subtree_data.template get<Self>().value;
  }

  /**
   * @brief Gets the sum of values in a subtree.
   *
   * Helper function to access the sum of values in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to get sum from.
   * @return The sum of values in the subtree.
   */
  template <typename TNode>
  static constexpr SumType get(const TNode* node) {
    return node ? get(node->subtree_data) : SumType{};
  }

  /**
   * @brief Gets a reference to the sum value in a subtree.
   *
   * Helper function to get a mutable reference to the sum value stored in a
   * node. This is useful when you need to modify the sum value directly.
   * Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to get sum reference from.
   * @return A reference to the sum value stored in the node.
   */
  template <typename TNode>
  static constexpr SumType& get_ref(TNode* node) noexcept {
    assert(node);
    return node->subtree_data.template get<Self>().value;
  }

  /**
   * @brief Sets the sum of values in a subtree.
   *
   * Helper function to set the sum of values in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to set sum for.
   * @param value The new sum value to set.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, const SumType& value) {
    assert(node);
    node->subtree_data.template get<Self>().value = value;
  }

  /**
   * @brief Sets sum to the node's value.
   *
   * For a single node, the sum is equal to its value.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set sum from.
   */
  template <typename TNode>
  constexpr void set_node(const TNode* node) {
    assert(node);
    value = SumType(node->data);
  }

  /**
   * @brief Copies sum from another subtree.
   *
   * Used when replacing one subtree with another to maintain correct sum.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to copy sum from.
   */
  template <typename TNode>
  constexpr void set_subtree(const TNode* node) {
    assert(node);
    value = get(node);
  }

  /**
   * @brief Adds node's value to the sum.
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
    value += SumType(node->data);
  }

  /**
   * @brief Adds sum of another subtree.
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
    value += get(node);
  }

  /**
   * @brief Adds node's value during insertion.
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
    value += SumType(node->data);
  }

  /**
   * @brief Adds sum of inserted subtree.
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
    value += get(node);
  }

  /**
   * @brief Subtracts node's value during removal.
   *
   * Updates the subtree sum when a node is being removed
   * from the tree.
   *
   * @tparam TNode The BST node type.
   * @param node The node being removed.
   */
  template <typename TNode>
  constexpr void remove_node(const TNode* node) {
    assert(node);
    value -= SumType(node->data);
  }

 protected:
  /**
   * @brief The sum of values in the subtree.
   *
   * This value represents the total sum of node values in the subtree,
   * including the root node.
   */
  SumType value{};
};

}  // namespace subtree_data
}  // namespace bst
