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
 * - Tree building and manipulation operations
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

  /**
   * @brief Operation support flags.
   *
   * These flags determine which operations are supported by the tree:
   * - support_insert: Whether insertion operations are supported
   * - support_remove: Whether removal operations are supported
   * - support_remove_by_node: Whether node-based removal is supported
   * - support_join: Whether two-way join operations are supported
   * - support_split: Whether split operations are supported
   */
  static constexpr bool support_insert = true;
  static constexpr bool support_remove = true;
  static constexpr bool support_remove_by_node =
      Derived::support_remove && has_parent;
  static constexpr bool support_join = true;
  static constexpr bool support_split = true;

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
    return create_node_impl<false>(data);
  }

  /**
   * @brief Creates a new node with the given data and key.
   *
   * @param data The data to store in the new node
   * @param key The key value for the node
   * @return Pointer to the newly created node
   */
  constexpr NodeType* create_node(const DataType& data, const KeyType& key) {
    return create_node_impl<false>(data, key);
  }

  /**
   * @brief Builds a tree from a vector of nodes.
   *
   * This function constructs a balanced binary search tree from a vector of
   * nodes. The nodes should be sorted by key if the tree uses keys.
   *
   * @param nodes Vector of nodes to build the tree from
   * @return Pointer to the root of the built tree
   */
  static constexpr NodeType* build_tree(const std::vector<NodeType*>& nodes) {
    return Derived::template build_tree_impl<false>(nodes);
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
    for (size_t i = 0; i < data.size(); ++i)
      nodes[i] = create_node_impl<true>(data[i]);
    return Derived::template build_tree_impl<true>(nodes);
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
      nodes[i] = create_node_impl<true>(data[i], keys[i]);
    std::sort(
        nodes.begin(), nodes.end(),
        [](const NodeType* a, const NodeType* b) { return a->key < b->key; });
    return Derived::template build_tree_impl<true>(nodes);
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
    return Derived::insert(root, create_node_impl<false>(data, key));
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
    return Derived::insert_at(root, create_node_impl<false>(data), index);
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
   * @brief Creates a new node with the given data.
   *
   * @tparam skip_update Whether to skip updating subtree data
   * @param data The data to store in the new node
   * @return Pointer to the newly created node
   */
  template <bool skip_update>
  [[nodiscard]] constexpr NodeType* create_node_impl(const DataType& data) {
    auto p = nodes_manager_.create();
    p->data = data;
    p->subtree_data.bti_reset();
    if constexpr (!skip_update) p->update_subtree_data();
    return p;
  }

  /**
   * @brief Creates a new node with the given data and key.
   *
   * @tparam skip_update Whether to skip updating subtree data
   * @param data The data to store in the new node
   * @param key The key value for the node
   * @return Pointer to the newly created node
   */
  template <bool skip_update>
  [[nodiscard]] constexpr NodeType* create_node_impl(const DataType& data,
                                                     const KeyType& key) {
    static_assert(has_key, "has_key should be true");
    auto p = nodes_manager_.create();
    p->data = data;
    p->key = key;
    p->subtree_data.bti_reset();
    if constexpr (!skip_update) p->update_subtree_data();
    return p;
  }

  /**
   * @brief Builds a balanced binary search tree from a range of nodes.
   *
   * @tparam update_leafs Whether to update subtree data for leaf nodes
   * @param nodes Vector of nodes to build the tree from
   * @param begin Start index of the range
   * @param end End index of the range (exclusive)
   * @return Pointer to the root of the built subtree
   */
  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_base_impl(
      const std::vector<NodeType*>& nodes, size_t begin, size_t end) {
    if (begin >= end) return nullptr;
    const size_t m = (begin + end) / 2;
    NodeType* root = nodes[m];
    root->set_left(build_tree_base_impl<update_leafs>(nodes, begin, m));
    root->set_right(build_tree_base_impl<update_leafs>(nodes, m + 1, end));
    if (update_leafs || (end - begin > 1)) root->update_subtree_data();
    return root;
  }

  /**
   * @brief Builds a balanced binary search tree from a vector of nodes.
   *
   * @tparam update_leafs Whether to update subtree data for leaf nodes
   * @param nodes Vector of nodes to build the tree from
   * @return Pointer to the root of the built tree
   */
  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_impl(
      const std::vector<NodeType*>& nodes) {
    NodeType* root = Derived::template build_tree_base_impl<update_leafs>(
        nodes, 0, nodes.size());
    if (root) root->set_parent(nullptr);
    return root;
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
