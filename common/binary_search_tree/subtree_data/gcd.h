#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"
#include "common/numeric/utils/gcd.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the GCD of node values in a BST.
 *
 * This component tracks the greatest common divisor (GCD) in each subtree
 * and supports basic operations including insertion and segment operations.
 * The GCD is maintained efficiently through local updates during tree
 * modifications.
 *
 * @tparam GCDType The type used for GCD values.
 */
template <typename GCDType>
class GCD : public Base {
 public:
  using Self = GCD<GCDType>;

  /**
   * @brief Component capability flags.
   *
   * GCD component supports most operations and requires access to node data
   * to maintain the GCD value. Remove operations are not supported as
   * they would require recomputation of the GCD.
   */
  static constexpr bool use_data = true;
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;

  /**
   * @brief Gets the GCD from a SubtreeData instance.
   *
   * Helper function to access the GCD directly from a SubtreeData object.
   * Assumes that GCD is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get GCD from.
   * @return The GCD stored in the subtree data.
   */
  template <typename TAggregators>
  static constexpr GCDType get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) {
    return subtree_data.template get<Self>().value;
  }

  /**
   * @brief Gets the GCD of values in a subtree.
   *
   * Helper function to access the GCD of values in a subtree through its root
   * node. Returns default-constructed GCDType for null nodes.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to get GCD from.
   * @return The GCD of values in the subtree.
   */
  template <typename TNode>
  static constexpr GCDType get(const TNode* node) {
    return node ? get(node->subtree_data) : GCDType{};
  }

  /**
   * @brief Sets the GCD of values in a subtree.
   *
   * Helper function to set the GCD of values in a subtree through its root
   * node. Assumes the node exists.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to set GCD for.
   * @param value The new GCD value to set.
   */
  template <typename TNode>
  static constexpr void set(TNode* node, const GCDType& value) {
    assert(node);
    node->subtree_data.template get<Self>().value = value;
  }

  /**
   * @brief Sets GCD to the node's value.
   *
   * For a single node, the GCD is equal to its value.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set GCD from.
   */
  template <typename TNode>
  constexpr void set_node(const TNode* node) {
    assert(node);
    value = GCDType(node->data);
  }

  /**
   * @brief Copies GCD from another subtree.
   *
   * Used when replacing one subtree with another to maintain correct GCD.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to copy GCD from.
   */
  template <typename TNode>
  constexpr void set_subtree(const TNode* node) {
    assert(node);
    value = get(node);
  }

  /**
   * @brief Updates GCD with node's value.
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
    value = ::GCD(value, GCDType(node->data));
  }

  /**
   * @brief Updates GCD with another subtree's GCD.
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
    value = ::GCD(value, get(node));
  }

  /**
   * @brief Updates GCD with node's value during insertion.
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
    value = ::GCD(value, GCDType(node->data));
  }

  /**
   * @brief Updates GCD with inserted subtree's GCD.
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
    value = ::GCD(value, get(node));
  }

 protected:
  /**
   * @brief The GCD value in the subtree.
   *
   * This value represents the greatest common divisor of all node values
   * in the subtree, including the root node.
   */
  GCDType value{};
};

}  // namespace subtree_data
}  // namespace bst
