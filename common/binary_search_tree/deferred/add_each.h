#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/base.h"
#include "common/binary_search_tree/subtree_data/max.h"
#include "common/binary_search_tree/subtree_data/min.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"

namespace bst {
namespace deferred {

// Forward declaration of the helper function
template <class Node, typename ValueType>
constexpr void add_to_each(Node* node, const ValueType& value);

/**
 * @brief A deferred computation manager for adding a value to each node's data
 * in a BST.
 *
 * This class manages deferred operations that add a value to the data of each
 * node in a subtree. It allows multiple add operations to be queued and applied
 * efficiently by deferring actual additions to child nodes until necessary.
 *
 * The class maintains a value that needs to be added to each node's data.
 * When applying additions, it adds the value to the current node's data and
 * propagates the addition request to child nodes. This lazy approach allows
 * for efficient handling of multiple add operations.
 *
 * If the node's subtree data includes aggregators, they will be updated:
 * - Sum: adds (value * subtree_size) to maintain the sum
 * - Max: adds value to maintain the maximum
 * - Min: adds value to maintain the minimum
 * These updates are checked at compile-time using the node's TInfo type.
 *
 * @tparam ValueType The type of values to be added to node data.
 */
template <typename ValueType>
class AddEach : public Base {
 public:
  using Self = AddEach;
  /** @brief Shorthand for Sum aggregator with matching value type. */
  using SDSum = bst::subtree_data::Sum<ValueType>;
  /** @brief Shorthand for Max aggregator with matching value type. */
  using SDMax = bst::subtree_data::Max<ValueType>;
  /** @brief Shorthand for Min aggregator with matching value type. */
  using SDMin = bst::subtree_data::Min<ValueType>;

  /**
   * @brief Flag indicating if deferred operations modify node data.
   *
   * Set to true because adding a value modifies the data stored in
   * tree nodes.
   */
  static constexpr bool modify_data = true;

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
  constexpr bool apply_required() const {
    return deferred_value != ValueType{};
  }

  /**
   * @brief Adds a value to be applied to each node's data in the subtree.
   *
   * This function queues a value to be added to each node's data in the
   * subtree. The actual addition will be deferred until apply() is called.
   *
   * If the node's SubtreeDataType includes aggregators, they will be updated
   * immediately:
   * - Sum: adds (value * subtree_size) to maintain the sum
   * - Max: adds value to maintain the maximum
   * - Min: adds value to maintain the minimum
   * These checks are performed at compile-time.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to add value to.
   * @param value The value to be added to each node's data.
   */
  template <class Node>
  constexpr void add_value(Node* node, const ValueType& value) {
    deferred_value += value;
    // Update Sum aggregator if present in node's TInfo
    if constexpr (Node::SubtreeDataType::template has<SDSum>()) {
      SDSum::get_ref(node) += value * bst::subtree_data::size(node);
    }
    // Update Max aggregator if present
    if constexpr (Node::SubtreeDataType::template has<SDMax>()) {
      SDMax::get_ref(node) += value;
    }
    // Update Min aggregator if present
    if constexpr (Node::SubtreeDataType::template has<SDMin>()) {
      SDMin::get_ref(node) += value;
    }
  }

  /**
   * @brief Applies pending value additions.
   *
   * If a value needs to be added:
   * 1. Adds the value to the current node's data
   * 2. Propagates the addition request to child nodes
   * 3. Clears the value to be added
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to apply additions to.
   */
  template <class Node>
  constexpr void apply(Node* node) {
    assert(node);
    if (deferred_value != ValueType{}) {
      node->data += deferred_value;
      add_to_each(node->left, deferred_value);
      add_to_each(node->right, deferred_value);
      deferred_value = ValueType{};
    }
  }

  /**
   * @brief Static interface for adding a value to each node's data.
   *
   * This is the standardized way to queue an add operation.
   * The value parameter specifies the amount to add to each node's data.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to modify.
   * @param value The value to add to each node's data.
   */
  template <class Node>
  static constexpr void add(Node* node, const ValueType& value) {
    add_to_each(node, value);
  }

 protected:
  /**
   * @brief The value to be added to each node's data.
   *
   * This value represents the amount that needs to be added to each
   * node's data in the subtree. Multiple calls to add_value() will
   * accumulate this value, allowing for efficient handling of multiple
   * additions.
   */
  ValueType deferred_value{};
};

/**
 * @brief Helper function to add a value to each data value in a subtree.
 *
 * This function provides a convenient way to request adding a value to
 * each node's data in a subtree through the node's deferred computation
 * manager. The actual addition will be performed when deferred computations
 * are applied.
 *
 * @tparam Node The BST node type.
 * @tparam ValueType The type of value to add.
 * @param node The root of the subtree to add value to.
 * @param value The value to add to each node's data.
 */
template <class Node, typename ValueType>
constexpr void add_to_each(Node* node, const ValueType& value) {
  if (node) {
    node->deferred.template get<AddEach<ValueType>>().add_value(node, value);
  }
}

}  // namespace deferred
}  // namespace bst
