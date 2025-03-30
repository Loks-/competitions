#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/base/find.h"
#include "common/binary_search_tree/subtree_data/size.h"

#include <vector>

namespace bst {
namespace base {
/**
 * @brief Base class for basic binary search tree operations.
 *
 * This class provides core functionality for binary search trees:
 * - Type definitions and feature flags
 * - Node management (creation and release)
 * - Basic tree operations that require access to node manager
 *
 * @tparam NodesManager The node manager type that handles node allocation and
 * management
 * @tparam Derived The derived class type (for CRTP)
 */
template <typename NodesManager, typename Derived>
class BasicTree {
 public:
  /**
   * @brief Type aliases for better readability and type safety.
   */
  using NodesManagerType = NodesManager;
  using NodeType = typename NodesManagerType::NodeType;
  using DataType = typename NodeType::DataType;
  using KeyType = typename NodeType::KeyType;
  using SubtreeDataType = typename NodeType::SubtreeDataType;
  using DeferredType = typename NodeType::DeferredType;

  /**
   * @brief Node feature flags inherited from NodeType.
   */
  static constexpr bool has_key = NodeType::has_key;
  static constexpr bool has_parent = NodeType::has_parent;

  /**
   * @brief Operation support flags.
   *
   * These flags determine which operations are supported by the tree:
   * - support_insert: Whether insertion operations are supported
   * - support_remove: Whether removal operations are supported (requires parent
   * links)
   * - support_remove_by_node: Whether node-based removal is supported
   * - support_join3: Whether three-way join operations are supported
   * - support_join: Whether two-way join operations are supported
   * - support_split: Whether split operations are supported
   * - support_insert_at: Whether order-based insertion is supported
   */
  static constexpr bool support_insert = true;
  static constexpr bool support_remove = has_parent;
  static constexpr bool support_remove_by_node =
      has_parent && Derived::support_remove;
  static constexpr bool support_join3 = false;
  static constexpr bool support_join =
      Derived::support_join3 && Derived::support_remove_by_node;
  static constexpr bool support_split = Derived::support_join3;
  static constexpr bool support_insert_at =
      !has_key && SubtreeDataType::has_size && Derived::support_join3 &&
      Derived::support_split;

  static constexpr bool is_persistent = false;

  /**
   * @brief Constructs a tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  explicit constexpr BasicTree(size_t max_nodes) : nodes_manager_(max_nodes) {}

  /**
   * @brief Default constructor.
   *
   * Creates a tree with no initial capacity.
   */
  constexpr BasicTree() : BasicTree(0) {}

  /**
   * @brief Returns a pointer to the derived class instance.
   *
   * This helper is used to access derived class methods through CRTP.
   *
   * @return Pointer to the derived class instance
   */
  [[nodiscard]] constexpr Derived* derived() {
    return static_cast<Derived*>(this);
  }

  [[nodiscard]] constexpr const Derived* derived() const {
    return static_cast<const Derived*>(this);
  }

  /**
   * @brief Creates a new node with the given data.
   *
   * @param data The data to store in the new node
   * @return Pointer to the newly created node
   */
  NodeType* create_node(const DataType& data) {
    auto p = create_empty_node();
    p->data = data;
    p->subtree_data.bti_reset();
    p->update_subtree_data();
    return p;
  }

  /**
   * @brief Creates a new node with the given data and key.
   *
   * @param data The data to store in the new node
   * @param key The key value for the node
   * @return Pointer to the newly created node
   */
  NodeType* create_node(const DataType& data, const KeyType& key) {
    static_assert(has_key, "has_key should be true");
    auto p = create_empty_node();
    p->data = data;
    p->key = key;
    p->subtree_data.bti_reset();
    p->update_subtree_data();
    return p;
  }

  /**
   * @brief Releases a node back to the node manager.
   *
   * @param node The node to release
   */
  void release(NodeType* node) { nodes_manager_.release(node); }

  /**
   * @brief Recursively releases all nodes in a tree.
   *
   * @param root The root of the tree to release
   */
  void release_tree(NodeType* root) {
    if (root) {
      release_tree(root->left);
      release_tree(root->right);
      release(root);
    }
  }

  /**
   * @brief Clears all nodes from the node manager.
   */
  void clear() { nodes_manager_.clear(); }

  /**
   * @brief Initializes the node manager with a new capacity.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  void init(size_t max_nodes) { nodes_manager_.init(max_nodes); }

  /**
   * @brief Returns the number of nodes currently in use.
   *
   * @return The number of used nodes
   */
  [[nodiscard]] constexpr size_t used() const { return nodes_manager_.used(); }

  /**
   * @brief Gets a node at a specific index from the node manager.
   *
   * @param index The index of the node to retrieve
   * @return Pointer to the node at the given index
   */
  [[nodiscard]] NodeType* manager_at(size_t index) {
    static_assert(NodesManagerType::support_at,
                  "nodes manager should support at");
    return nodes_manager_.at(index);
  }

  /**
   * @brief Gets a const node at a specific index from the node manager.
   *
   * @param index The index of the node to retrieve
   * @return Const pointer to the node at the given index
   */
  [[nodiscard]] const NodeType* manager_at(size_t index) const {
    static_assert(NodesManagerType::support_at,
                  "nodes manager should support at");
    return nodes_manager_.at(index);
  }

  /**
   * @brief Gets the index of a node in the node manager.
   *
   * @param node The node to find the index for
   * @return The index of the node
   */
  [[nodiscard]] size_t manager_index(const NodeType* node) const {
    static_assert(NodesManagerType::support_index,
                  "nodes manager should support index");
    return nodes_manager_.index(node);
  }

 protected:
  /**
   * @brief Creates an empty node without initializing data.
   *
   * @return Pointer to the newly created empty node
   */
  [[nodiscard]] NodeType* create_empty_node() {
    return nodes_manager_.create();
  }

 protected:
  NodesManagerType nodes_manager_;
};
}  // namespace base
}  // namespace bst