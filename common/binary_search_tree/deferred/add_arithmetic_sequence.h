#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/base.h"
#include "common/binary_search_tree/deferred/reverse.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"

#include <utility>

namespace bst {
namespace deferred {

// Forward declaration of the helper function
template <typename TNode, typename ValueType>
constexpr void add_arithmetic_sequence(TNode* node, const ValueType& a,
                                       const ValueType& d);

/**
 * @brief A deferred computation manager for adding arithmetic sequence to each
 * node's data in a BST.
 *
 * This class manages deferred operations for adding an arithmetic sequence
 * to each node's data in a subtree. The sequence starts with value 'a'
 * and increases by 'd' for each subsequent node in the subtree.
 *
 * The class maintains the sequence parameters (a, d) and applies them
 * when deferred computations are processed. The sequence is applied
 * in-order to ensure correct sequence of values.
 */
template <typename ValueType>
class AddArithmeticSequence : public Base {
 public:
  using Self = AddArithmeticSequence<ValueType>;
  using SDSum = bst::subtree_data::Sum<ValueType>;

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
   * Clears the sequence parameters.
   */
  constexpr void clear() {
    a = ValueType{};
    d = ValueType{};
  }

  /**
   * @brief Checks if there are any deferred computations that need to be
   * applied.
   *
   * @return true if there are sequence parameters to apply, false otherwise.
   */
  constexpr bool apply_required() const noexcept {
    return (a != ValueType{}) || (d != ValueType{});
  }

  /**
   * @brief Queues a value for addition to each node's data in arithmetic
   * sequence.
   *
   * This function queues a value to be added to each node's data in the
   * subtree. The value will be added in arithmetic sequence starting with 'a'
   * and increasing by 'd' for each subsequent node.
   *
   * The sequence parameters are accumulated with any existing deferred
   * operations, as we don't know the state of previous deferred operations at
   * the time of call.
   *
   * If the node has a Sum aggregator, it is updated immediately with the sum of
   * the arithmetic sequence: size * a + size * (size - 1) * d / 2
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to add sequence to.
   * @param a_ The starting value of the sequence to add.
   * @param d_ The common difference of the sequence to add.
   */
  template <typename TNode>
  constexpr void add_sequence(TNode* node, const ValueType& a_,
                              const ValueType& d_) {
    a += a_;
    d += d_;

    // Update Sum aggregator if present
    if constexpr (TNode::SubtreeDataType::template has<SDSum>()) {
      const auto subtree_size = ValueType(bst::subtree_data::size(node));
      SDSum::get_ref(node) += subtree_size * a_ + subtree_size *
                                                      (subtree_size - 1) * d_ /
                                                      ValueType(2);
    }
  }

  /**
   * @brief Applies pending arithmetic sequence additions.
   *
   * The arithmetic sequence is applied in-order (from left to right) within the
   * subtree. For a node with left subtree of size L:
   * 1. Left subtree gets sequence starting with 'a' and difference 'd'
   * 2. Current node gets value 'a + L * d'
   * 3. Right subtree gets sequence starting with 'a + (L + 1) * d' and
   * difference 'd'
   *
   * Before applying the sequence, any tree structure modifications (like
   * reversals) are applied first to ensure correct tree structure.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to apply sequence to.
   */
  template <typename TNode>
  constexpr void apply(TNode* node) {
    assert(node);
    if (apply_required()) {
      // Apply any tree structure modifications first
      if constexpr (TNode::DeferredType::template has<Reverse>())
        node->deferred.template get<Reverse>().apply(node);

      const auto left_size = ValueType(bst::subtree_data::size(node->left));
      add_arithmetic_sequence(node->left, a, d);
      node->data += a + left_size * d;
      add_arithmetic_sequence(node->right, a + (left_size + 1) * d, d);
      clear();
    }
  }

  /**
   * @brief Static interface for adding a sequence request.
   *
   * This is the standardized way to queue a sequence operation.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to add sequence to.
   * @param params A pair containing the starting value (first) and common
   * difference (second) of the sequence.
   */
  template <typename TNode>
  static constexpr void add(TNode* node,
                            const std::pair<ValueType, ValueType>& params) {
    bst::deferred::add_arithmetic_sequence(node, params.first, params.second);
  }

  /**
   * @brief Updates sequence parameters when a subtree is reversed.
   *
   * When a subtree is reversed, the arithmetic sequence needs to be adjusted:
   * - The starting value becomes the last value of the original sequence
   * - The common difference becomes negative of the original difference
   *
   * @tparam TNode The BST node type.
   * @param node The root of the reversed subtree.
   */
  template <typename TNode>
  constexpr void reverse_subtree(TNode* node) {
    if (apply_required()) {
      const auto subtree_size = ValueType(bst::subtree_data::size(node));
      a += (subtree_size - 1) * d;  // Make a the last value of the sequence
      d = -d;                       // Reverse the direction of the sequence
    }
  }

 protected:
  /**
   * @brief The starting value of the arithmetic sequence.
   */
  ValueType a{};

  /**
   * @brief The common difference of the arithmetic sequence.
   */
  ValueType d{};
};

/**
 * @brief Helper function to add arithmetic sequence to each node's data in a
 * subtree.
 *
 * This function provides a convenient way to request adding an arithmetic
 * sequence to each node's data through the node's deferred computation
 * manager.
 *
 * @tparam TNode The BST node type.
 * @tparam ValueType The type of values in the sequence.
 * @param node The root of the subtree to add sequence to.
 * @param a The starting value of the sequence.
 * @param d The common difference of the sequence.
 */
template <typename TNode, typename ValueType>
constexpr void add_arithmetic_sequence(TNode* node, const ValueType& a,
                                       const ValueType& d) {
  if (node) {
    node->deferred.template get<AddArithmeticSequence<ValueType>>()
        .add_sequence(node, a, d);
  }
}

}  // namespace deferred
}  // namespace bst