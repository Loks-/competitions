#pragma once

#include "common/base.h"

#include <algorithm>
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
  static constexpr bool has_size = SubtreeDataType::has_size;

  static constexpr bool is_persistent = false;

 public:
  /**
   * @brief Constructs a tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr BasicTree(size_t max_nodes)
      : nodes_manager_(max_nodes) {}

  /**
   * @brief Default constructor.
   *
   * Creates a tree with no initial capacity.
   */
  [[nodiscard]] constexpr BasicTree() : BasicTree(0) {}

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
  constexpr NodeType* create_node(const DataType& data) {
    auto p = nodes_manager_.create();
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
  constexpr NodeType* create_node(const DataType& data, const KeyType& key) {
    static_assert(has_key, "has_key should be true");
    auto p = nodes_manager_.create();
    p->data = data;
    p->key = key;
    p->subtree_data.bti_reset();
    p->update_subtree_data();
    return p;
  }

  /**
   * @brief Builds a tree from a vector of data elements.
   *
   * @param data Vector of data elements to build the tree from
   * @return Pointer to the root of the built tree
   */
  [[nodiscard]] constexpr NodeType* build(const std::vector<DataType>& data) {
    if (data.empty()) return nullptr;
    nodes_manager_.reserve_additional(data.size());
    std::vector<NodeType*> nodes(data.size());
    for (size_t i = 0; i < data.size(); ++i) nodes[i] = create_node(data[i]);
    return Derived::build_tree(nodes);
  }

  /**
   * @brief Builds a tree from vectors of data and keys.
   *
   * @param data Vector of data elements to build the tree from
   * @param keys Vector of keys corresponding to the data elements
   * @return Pointer to the root of the built tree
   */
  [[nodiscard]] constexpr NodeType* build(const std::vector<DataType>& data,
                                          const std::vector<KeyType>& keys) {
    static_assert(has_key, "has_key should be true");
    assert(data.size() == keys.size());
    if (data.empty()) return nullptr;
    nodes_manager_.reserve_additional(data.size());
    std::vector<NodeType*> nodes(data.size());
    for (size_t i = 0; i < data.size(); ++i)
      nodes[i] = create_node(data[i], keys[i]);
    std::sort(
        nodes.begin(), nodes.end(),
        [](const NodeType* a, const NodeType* b) { return a->key < b->key; });
    return Derived::build_tree(nodes);
  }

  /**
   * @brief Creates and inserts a new node with the given data and key.
   *
   * @param root The root of the tree
   * @param data The data for the new node
   * @param key The key for the new node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] constexpr NodeType* insert_new(NodeType* root,
                                               const DataType& data,
                                               const KeyType& key) {
    return Derived::insert(root, create_node(data, key));
  }

  /**
   * @brief Creates and inserts a new node at the specified inorder index.
   *
   * @param root The root of the tree
   * @param data The data for the new node
   * @param index The zero-based index where to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] constexpr NodeType* insert_new_at(NodeType* root,
                                                  const DataType& data,
                                                  size_t index) {
    return Derived::insert_at(root, create_node(data), index);
  }

  /**
   * @brief Releases a node back to the node manager.
   *
   * @param node The node to release
   */
  constexpr void release(NodeType* node) {
    assert(node);
    nodes_manager_.release(node);
  }

  /**
   * @brief Recursively releases all nodes in a tree.
   *
   * @param root The root of the tree to release
   */
  constexpr void release_tree(NodeType* root) {
    if (root) {
      release_tree(root->left);
      release_tree(root->right);
      release(root);
    }
  }

  /**
   * @brief Removes a node and releases it back to the node manager.
   *
   * @param node The node to remove and release
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] constexpr NodeType* remove_and_release_node(NodeType* node) {
    NodeType* new_root = Derived::remove_node(node);
    release(node);
    return new_root;
  }

  /**
   * @brief Removes a node with the given key and releases it.
   *
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] constexpr NodeType* remove_and_release(NodeType* root,
                                                       const KeyType& key) {
    NodeType* removed_node = nullptr;
    NodeType* new_root = Derived::remove(root, key, removed_node);
    if (removed_node) release(removed_node);
    return new_root;
  }

  /**
   * @brief Removes a node at the specified inorder index and releases it.
   *
   * @param root The root of the tree
   * @param index The zero-based index of the node to remove
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] constexpr NodeType* remove_and_release_at(NodeType* root,
                                                          size_t index) {
    NodeType* removed_node = nullptr;
    NodeType* new_root = Derived::remove_at(root, index, removed_node);
    if (removed_node) release(removed_node);
    return new_root;
  }

  /**
   * @brief Clears all nodes from the node manager.
   */
  constexpr void clear() { nodes_manager_.clear(); }

  /**
   * @brief Initializes the node manager with a new capacity.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  constexpr void init(size_t max_nodes) { nodes_manager_.init(max_nodes); }

  /**
   * @brief Returns the number of nodes currently in use.
   *
   * @return The number of used nodes
   */
  [[nodiscard]] constexpr size_t used() const noexcept {
    return nodes_manager_.used();
  }

  /**
   * @brief Gets a node at a specific index from the node manager.
   *
   * @param index The index of the node to retrieve
   * @return Pointer to the node at the given index
   */
  [[nodiscard]] constexpr NodeType* manager_at(size_t index) {
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
  [[nodiscard]] constexpr const NodeType* manager_at(size_t index) const {
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
  [[nodiscard]] constexpr size_t manager_index(const NodeType* node) const {
    static_assert(NodesManagerType::support_index,
                  "nodes manager should support index");
    return nodes_manager_.index(node);
  }

 protected:
  /**
   * @brief Node manager instance that handles node allocation and management.
   *
   * This member is protected to allow derived classes to access the node
   * manager for their specific implementations while keeping it hidden from
   * external users.
   */
  NodesManagerType nodes_manager_;
};
}  // namespace base
}  // namespace bst
