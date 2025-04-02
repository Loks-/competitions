#pragma once

#include "common/base.h"
#include "common/memory/node.h"
#include "common/template.h"

#include <type_traits>

namespace bst {
namespace base {

/**
 * @brief Main BST node class that combines all functionality.
 *
 * This class represents a node in a binary search tree with the following
 * features:
 * - Optional key storage (controlled by has_key_)
 * - Optional parent links (controlled by has_parent_)
 * - Data storage
 * - Subtree data aggregation
 * - Deferred computations
 *
 * The class provides a flexible foundation for building various types of
 * binary search trees by allowing optional features through template
 * parameters. It maintains the tree structure and manages both immediate
 * and deferred operations on the data.
 *
 * @tparam Data Type of data stored in the node
 * @tparam SubtreeData_ Type of subtree data aggregator
 * @tparam Deferred_ Type of deferred computations
 * @tparam has_parent_ Whether to maintain parent links
 * @tparam has_key_ Whether to store keys in nodes
 * @tparam Key Type of keys (if has_key_ is true)
 */
template <typename Data, typename SubtreeData_, typename Deferred_,
          bool has_parent_ = true, bool has_key_ = false,
          typename Key = int64_t>
class Node : public memory::Node {
 public:
  /**
   * @brief Type aliases for better readability and type safety.
   *
   * These aliases provide clear names for the various types used in the node,
   * making the code more maintainable and easier to understand.
   */
  using DataType = Data;                 // Type of data stored in the node
  using SubtreeDataType = SubtreeData_;  // Type of subtree data aggregator
  using DeferredType = Deferred_;        // Type of deferred computations
  using KeyType = Key;                   // Type of keys (if has_key is true)
  using Self = Node;                     // Type of the node itself

  /**
   * @brief Feature flags for node capabilities.
   *
   * These flags determine the node's structure and capabilities:
   * - has_parent: Whether the node maintains parent links
   * - has_key: Whether the node stores keys
   */
  static constexpr bool has_parent =
      has_parent_;                           // Whether node has parent links
  static constexpr bool has_key = has_key_;  // Whether node stores keys

  /**
   * @brief Operation support flags.
   *
   * These flags indicate which operations are supported by the node.
   * Currently set to false as they will be handled by SubtreeData.
   */
  static constexpr bool support_insert =
      false;  // Whether node supports insertion
  static constexpr bool support_remove =
      false;  // Whether node supports removal

 public:
  /**
   * @brief Core data members of the node.
   *
   * These members store the actual data and manage the node's state:
   * - data: The primary data stored in the node
   * - subtree_data: Aggregated data for the entire subtree
   * - deferred: Pending computations to be applied
   */
  DataType data;                   // Data stored in the node
  SubtreeDataType subtree_data{};  // Aggregated data for the subtree
  DeferredType deferred{};         // Deferred computations

  /**
   * @brief Optional key member for key-based operations.
   *
   * This member is either KeyType or MetaEmpty based on has_key flag.
   */
  std::conditional_t<has_key, KeyType, MetaEmpty> key{};

  /**
   * @brief Tree structure members.
   *
   * These members maintain the tree structure:
   * - left: Pointer to the left child node
   * - right: Pointer to the right child node
   */
  Self* left{nullptr};   // Left child node
  Self* right{nullptr};  // Right child node

  /**
   * @brief Optional parent link for upward traversal.
   *
   * This member is either Self* or MetaEmpty based on has_parent flag.
   */
  std::conditional_t<has_parent, Self*, MetaEmpty> parent{};

 public:
  /**
   * @brief Default constructor.
   *
   * Initializes the node with default-constructed data.
   */
  constexpr Node() : data() {
    if constexpr (has_parent) {
      parent = nullptr;
    }
  }

  /**
   * @brief Constructor with initial data.
   *
   * Creates a node with the specified data value.
   *
   * @param data_ The initial data to store in the node
   */
  constexpr explicit Node(const DataType& data_) : data(data_) {
    if constexpr (has_parent) {
      parent = nullptr;
    }
  }

  /**
   * @brief Clears all deferred computations.
   *
   * Resets the deferred computation state to empty, ensuring no
   * pending operations remain.
   */
  constexpr void clear_deferred() { deferred.clear(); }

  /**
   * @brief Applies all pending deferred computations.
   *
   * This function should be called before accessing the node's data
   * to ensure all deferred operations are applied.
   */
  constexpr void apply_deferred() { deferred.apply(this); }

  /**
   * @brief Updates the subtree data aggregator.
   *
   * This function should be called whenever the node's data or its
   * subtree structure changes to maintain correct aggregated values.
   */
  constexpr void update_subtree_data() { subtree_data.update(this); }

  /**
   * @brief Resets all links and updates subtree data.
   *
   * This function is typically used when removing a node from the tree.
   * It resets all tree links and updates the subtree data to reflect
   * the removal.
   */
  constexpr void reset_links_and_update_subtree_data() {
    reset_links();
    subtree_data.bti_reset();
    update_subtree_data();
  }

  /**
   * @brief Initializes a newly allocated node.
   *
   * This method is called by NodesManager when memory is first allocated
   * for a node. It initializes all node fields to their default values.
   *
   * @param index A unique index assigned to this node by NodesManager
   */
  constexpr void initialize(unsigned index) { subtree_data.initialize(index); }

  /**
   * @brief Prepares a node for reuse from the free pool.
   *
   * This method is called by NodesManager when a node is being retrieved
   * from the free pool for reuse. It resets all linkes and deferred
   * computations.
   */
  constexpr void reuse() {
    reset_links();
    clear_deferred();
  }

  /**
   * @brief Sets the left child node.
   *
   * This function updates the left child link and maintains the
   * parent-child relationship if parent links are enabled.
   *
   * @param node The new left child node
   */
  constexpr void set_left(Self* node) noexcept {
    left = node;
    if constexpr (has_parent) {
      if (node) node->parent = this;
    }
  }

  /**
   * @brief Sets the right child node.
   *
   * This function updates the right child link and maintains the
   * parent-child relationship if parent links are enabled.
   *
   * @param node The new right child node
   */
  constexpr void set_right(Self* node) noexcept {
    right = node;
    if constexpr (has_parent) {
      if (node) node->parent = this;
    }
  }

  /**
   * @brief Sets the parent node.
   *
   * This function updates the parent link if parent links are enabled.
   *
   * @param node The new parent node
   */
  constexpr void set_parent(Self* node) noexcept {
    if constexpr (has_parent) {
      parent = node;
    }
  }

  /**
   * @brief Resets all tree links to nullptr.
   *
   * This function clears all tree structure links, including parent
   * link if parent links are enabled.
   */
  constexpr void reset_links() noexcept {
    left = right = nullptr;
    if constexpr (has_parent) {
      parent = nullptr;
    }
  }
};

}  // namespace base
}  // namespace bst
