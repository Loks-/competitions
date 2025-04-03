#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/base.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum_keys.h"

namespace bst {
namespace deferred {

// Forward declaration of the helper function
template <typename TNode, typename ValueType>
constexpr void add_to_each_key(TNode* node, const ValueType& value);

/**
 * @brief A deferred computation manager for adding a value to each node's key
 * in a BST.
 *
 * This class manages deferred operations that add a value to the key of each
 * node in a subtree. It allows multiple add operations to be queued and applied
 * efficiently by deferring actual additions to child nodes until necessary.
 *
 * The class maintains a value that needs to be added to each node's key.
 * When applying additions, it adds the value to the current node's key and
 * propagates the addition request to child nodes. This lazy approach allows
 * for efficient handling of multiple add operations.
 *
 * If the node's subtree data includes a SumKeys aggregator, this class will
 * automatically update the sum by adding (value * subtree_size) to maintain
 * consistency. This is checked at compile-time using the node's TInfo type.
 *
 * @tparam ValueType The type of values to be added to node keys.
 */
template <typename ValueType>
class AddEachKey : public Base {
 public:
  using Self = AddEachKey;
  /** @brief Shorthand for SumKeys aggregator with matching value type. */
  using SDSumKeys = bst::subtree_data::SumKeys<ValueType>;

  /**
   * @brief Flag indicating if deferred operations modify node keys.
   *
   * Set to true because adding a value modifies the keys stored in
   * tree nodes.
   */
  static constexpr bool modify_keys = true;

  /**
   * @brief Resets the deferred computation state to empty.
   *
   * Clears the value to be added.
   */
  constexpr void clear() { deferred_value = ValueType{}; }

  /**
   * @brief Checks if there are any deferred computations that need to be
   * applied.
   *
   * @return true if there is a non-zero value to be added, false otherwise.
   */
  constexpr bool apply_required() const noexcept {
    return deferred_value != ValueType{};
  }

  /**
   * @brief Adds a value to be applied to each node's key in the subtree.
   *
   * This function queues a value to be added to each node's key in the
   * subtree. The actual addition will be deferred until apply() is called.
   *
   * If the node's TInfo includes a SumKeys aggregator, its value will be
   * updated immediately by adding (value * subtree_size) to maintain the
   * correct sum of keys. This check is performed at compile-time.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to add value to.
   * @param value The value to be added to each node's key.
   */
  template <typename TNode>
  constexpr void add_value(TNode* node, const ValueType& value) {
    deferred_value += value;
    // Update SumKeys aggregator if present in node's SubtreeDataType
    if constexpr (TNode::SubtreeDataType::template has<SDSumKeys>())
      SDSumKeys::get_ref(node) += value * bst::subtree_data::size(node);
  }

  /**
   * @brief Applies pending value additions.
   *
   * If a value needs to be added:
   * 1. Adds the value to the current node's key
   * 2. Propagates the addition request to child nodes
   * 3. Clears the value to be added
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to apply additions to.
   */
  template <typename TNode>
  constexpr void apply(TNode* node) {
    assert(node);
    if (deferred_value != ValueType{}) {
      node->key += deferred_value;
      add_to_each_key(node->left, deferred_value);
      add_to_each_key(node->right, deferred_value);
      deferred_value = ValueType{};
    }
  }

  /**
   * @brief Static interface for adding a value to each node's key.
   *
   * This is the standardized way to queue an add operation.
   * The value parameter specifies the amount to add to each node's key.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to modify.
   * @param value The value to add to each node's key.
   */
  template <typename TNode>
  static constexpr void add(TNode* node, const ValueType& value) {
    add_to_each_key(node, value);
  }

 protected:
  /**
   * @brief The value to be added to each node's key.
   *
   * This value represents the amount that needs to be added to each
   * node's key in the subtree. Multiple calls to add_value() will
   * accumulate this value, allowing for efficient handling of multiple
   * additions.
   */
  ValueType deferred_value{};
};

/**
 * @brief Helper function to add a value to each key in a subtree.
 *
 * This function provides a convenient way to request adding a value to
 * each key in a subtree through the node's deferred computation manager.
 * The actual addition will be performed when deferred computations are
 * applied.
 *
 * @tparam TNode The BST node type.
 * @tparam ValueType The type of value to add.
 * @param node The root of the subtree to add value to.
 * @param value The value to add to each key.
 */
template <typename TNode, typename ValueType>
constexpr void add_to_each_key(TNode* node, const ValueType& value) {
  if (node) {
    node->deferred.template get<AddEachKey<ValueType>>().add_value(node, value);
  }
}

}  // namespace deferred
}  // namespace bst
