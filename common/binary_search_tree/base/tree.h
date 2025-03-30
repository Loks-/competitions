#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/base/find.h"
#include "common/binary_search_tree/subtree_data/size.h"

#include <algorithm>
#include <vector>

namespace bst {
namespace base {
/**
 * @brief Base class for binary search tree implementations.
 *
 * This class provides a foundation for implementing various types of binary
 * search trees. It combines node management with tree operations and supports
 * both key-based and order-based operations. The class uses CRTP to allow
 * derived classes to override specific operations while maintaining static
 * polymorphism.
 *
 * Key features:
 * - Node management through a configurable node manager
 * - Support for key-based and order-based operations
 * - Optional parent links for efficient tree traversal
 * - Subtree data aggregation
 * - Deferred computations
 *
 * @tparam NodesManager The node manager type that handles node allocation and
 * management
 * @tparam Derived The derived class type (for CRTP)
 */
template <typename NodesManager, typename Derived>
class Tree {
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
  explicit constexpr Tree(size_t max_nodes) : nodes_manager_(max_nodes) {}

  /**
   * @brief Default constructor.
   *
   * Creates a tree with no initial capacity.
   */
  constexpr Tree() : Tree(0) {}

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
  constexpr NodeType* create_node(const DataType& data, const KeyType& key) {
    static_assert(has_key, "has_key should be true");
    auto p = create_empty_node();
    p->data = data;
    p->key = key;
    p->subtree_data.bti_reset();
    p->update_subtree_data();
    return p;
  }

  /**
   * @brief Finds a node with the given key in the tree.
   *
   * @param root The root of the tree to search in
   * @param key The key to search for
   * @return Pointer to the node with the given key, or nullptr if not found
   */
  [[nodiscard]] static constexpr NodeType* find(NodeType* root,
                                                const KeyType& key) {
    return bst::base::find(root, key);
  }

  /**
   * @brief Gets the node at the specified inorder index.
   *
   * @param root The root of the tree
   * @param index The zero-based index to look up
   * @return Pointer to the node at the given index, or nullptr if index is out
   * of bounds
   */
  [[nodiscard]] static constexpr NodeType* at(NodeType* root, size_t index) {
    return bst::base::at(root, index);
  }

  /**
   * @brief Builds a tree from a vector of nodes.
   *
   * @param nodes Vector of node pointers to build the tree from
   * @return Pointer to the root of the newly built tree
   */
  [[nodiscard]] static constexpr NodeType* build_tree(
      const std::vector<NodeType*>& nodes) {
    NodeType* root = Derived::build_tree_impl(nodes, 0, nodes.size());
    if (root) root->set_parent(nullptr);
    return root;
  }

  /**
   * @brief Builds a tree from a vector of data elements.
   *
   * @param data Vector of data elements to create nodes from
   * @return Pointer to the root of the newly built tree
   */
  [[nodiscard]] constexpr NodeType* build_from_data(
      const std::vector<DataType>& data) {
    if (data.empty()) return nullptr;
    nodes_manager_.reserve_additional(data.size());
    std::vector<NodeType*> nodes(data.size());
    for (size_t i = 0; i < data.size(); ++i) nodes[i] = create_node(data[i]);
    return Derived::build_tree(nodes);
  }

  /**
   * @brief Builds a tree from vectors of data and keys.
   *
   * @param data Vector of data elements to create nodes from
   * @param keys Vector of keys corresponding to the data elements
   * @return Pointer to the root of the newly built tree
   */
  [[nodiscard]] constexpr NodeType* build_from_data(
      const std::vector<DataType>& data, const std::vector<KeyType>& keys) {
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
   * @brief Inserts a node into the tree.
   *
   * @param root The root of the tree to insert into
   * @param node The node to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* insert(NodeType* root,
                                                  NodeType* node) {
    static_assert(Derived::support_insert, "Insert should be supported");
    static_assert(has_key, "has_key should be true");
    assert(node);
    return root ? Derived::insert_impl(root, node) : node;
  }

  /**
   * @brief Inserts a node at the specified inorder index.
   *
   * @param root The root of the tree
   * @param node The node to insert
   * @param index The zero-based index where to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* insert_at(NodeType* root,
                                                     NodeType* node,
                                                     size_t index) {
    static_assert(support_insert_at);
    assert(node);
    if (!root) {
      assert(index == 0);
      return node;
    } else {
      assert(index <= bst::subtree_data::size(root));
      NodeType *l = nullptr, *r = nullptr;
      Derived::split_at(root, index, l, r);
      return Derived::join3(l, node, r);
    }
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
   * @brief Removes a node with the given key from the tree.
   *
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove(NodeType* root,
                                                  const KeyType& key,
                                                  NodeType*& removed_node) {
    static_assert(Derived::support_remove, "Remove should be supported");
    static_assert(has_key, "has_key should be true");
    return Derived::remove_impl(root, key, removed_node);
  }

  /**
   * @brief Removes the node at the specified inorder index.
   *
   * @param root The root of the tree
   * @param index The zero-based index of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_at(NodeType* root,
                                                     size_t index,
                                                     NodeType*& removed_node) {
    static_assert(Derived::support_remove, "Remove should be supported");
    return Derived::remove_at_impl(root, index, removed_node);
  }

  /**
   * @brief Removes a specific node from the tree.
   *
   * @param node The node to remove
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_node(NodeType* node) {
    static_assert(Derived::support_remove_by_node,
                  "Remove by node should be supported");
    static_assert(has_parent, "use_parent should be true");
    assert(node);
    return Derived::remove_node_impl(node);
  }

  /**
   * @brief Releases a node back to the node manager.
   *
   * @param node The node to release
   */
  void constexpr release(NodeType* node) { nodes_manager_.release(node); }

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
    NodeType *removed_node = nullptr,
             *new_root = Derived::remove(root, key, removed_node);
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
    NodeType *removed_node = nullptr,
             *new_root = Derived::remove_at(root, index, removed_node);
    if (removed_node) release(removed_node);
    return new_root;
  }

  /**
   * @brief Joins two trees together.
   *
   * @param l The left tree
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join(NodeType* l, NodeType* r) {
    return Derived::join_impl(l, r);
  }

  /**
   * @brief Joins two trees together with a middle node.
   *
   * @param l The left tree
   * @param m1 The middle node
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3(NodeType* l, NodeType* m1,
                                                 NodeType* r);

  /**
   * @brief Splits a tree at a given key.
   *
   * @param root The root of the tree to split
   * @param key The key to split at
   * @param output_l Reference to store the left part of the split
   * @param output_r Reference to store the right part of the split
   */
  static void constexpr split(NodeType* root, const KeyType& key,
                              NodeType*& output_l, NodeType*& output_r) {
    Derived::split_impl(root, key, output_l, output_r);
  }

  /**
   * @brief Splits a tree at a given inorder index.
   *
   * @param root The root of the tree to split
   * @param lsize The size of the left part
   * @param output_l Reference to store the left part of the split
   * @param output_r Reference to store the right part of the split
   */
  static void constexpr split_at(NodeType* root, size_t lsize,
                                 NodeType*& output_l, NodeType*& output_r);

  /**
   * @brief Recursively releases all nodes in a tree.
   *
   * @param root The root of the tree to release
   */
  void constexpr release_tree(NodeType* root) {
    if (root) {
      release_tree(root->left);
      release_tree(root->right);
      release(root);
    }
  }

  /**
   * @brief Clears all nodes from the node manager.
   */
  void constexpr clear() { nodes_manager_.clear(); }

  /**
   * @brief Initializes the node manager with a new capacity.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  void constexpr init(size_t max_nodes) { nodes_manager_.init(max_nodes); }

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
   * @brief Creates an empty node without initializing data.
   *
   * @return Pointer to the newly created empty node
   */
  [[nodiscard]] constexpr NodeType* create_empty_node() {
    return nodes_manager_.create();
  }

  /**
   * @brief Implementation of build_tree that can be overridden by derived
   * classes.
   *
   * @param nodes Vector of nodes to build the tree from
   * @param first Starting index in the nodes vector
   * @param last Ending index in the nodes vector
   * @return Pointer to the root of the built tree
   */
  [[nodiscard]] static constexpr NodeType* build_tree_impl(
      const std::vector<NodeType*>& nodes, size_t first, size_t last) {
    if (first >= last) return nullptr;
    size_t m = (first + last) / 2;
    NodeType* root = nodes[m];
    root->set_left(build_tree_impl(nodes, first, m));
    root->set_right(build_tree_impl(nodes, m + 1, last));
    root->update_subtree_data();
    return root;
  }

  /**
   * @brief Implementation of insert that can be overridden by derived classes.
   *
   * @param root The root of the tree
   * @param node The node to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* insert_impl(NodeType* root,
                                                       NodeType* node);

  /**
   * @brief Implementation of remove that can be overridden by derived classes.
   *
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_impl(
      NodeType* root, const KeyType& key, NodeType*& removed_node) {
    removed_node = Derived::find(root, key);
    return (removed_node ? Derived::remove_node_impl(removed_node) : root);
  }

  /**
   * @brief Implementation of remove_at that can be overridden by derived
   * classes.
   *
   * @param root The root of the tree
   * @param index The zero-based index of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_at_impl(
      NodeType* root, size_t index, NodeType*& removed_node) {
    removed_node = Derived::at(root, index);
    return (removed_node ? Derived::remove_node_impl(removed_node) : root);
  }

  /**
   * @brief Implementation of remove_node that can be overridden by derived
   * classes.
   *
   * @param node The node to remove
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_node_impl(NodeType* node);

  /**
   * @brief Implementation of join that can be overridden by derived classes.
   *
   * @param l The left tree
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join_impl(NodeType* l, NodeType* r) {
    static_assert(Derived::support_join, "Join should be supported");
    static_assert(Derived::support_remove_by_node,
                  "Remove by node should be supported");
    if (!l) return r;
    if (!r) return l;
    auto node = l;
    node->apply_deferred();
    while (node->right) {
      node = node->right;
      node->apply_deferred();
    }
    l = Derived::remove_node_impl(node);
    return Derived::join3(l, node, r);
  }

  /**
   * @brief Implementation of join3 that can be overridden by derived classes.
   *
   * This base implementation always uses m1 as the root of the joined tree,
   * connecting l as the left subtree and r as the right subtree. Derived
   * classes may override this to implement different balancing strategies.
   *
   * @param l The left tree
   * @param m1 The middle node that will become the root
   * @param r The right tree
   * @return Pointer to the new root of the joined tree (always m1 in this
   * implementation)
   */
  static constexpr NodeType* join3_impl(NodeType* l, NodeType* m1,
                                        NodeType* r) {
    m1->set_left(l);
    m1->set_right(r);
    m1->update_subtree_data();
    return m1;
  }

  /**
   * @brief Implementation of split that can be overridden by derived classes.
   *
   * @param root The root of the tree to split
   * @param key The key to split at
   * @param output_l Reference to store the left part of the split
   * @param output_r Reference to store the right part of the split
   */
  static constexpr void split_impl(NodeType* root, const KeyType& key,
                                   NodeType*& output_l, NodeType*& output_r) {
    static_assert(has_key, "has_key should be true");
    static_assert(Derived::support_join3, "Join3 should be supported");
    static_assert(Derived::support_split, "Split should be supported");
    if (!root) {
      output_l = output_r = nullptr;
      return;
    }
    root->apply_deferred();
    NodeType *l = root->left, *r = root->right, *m = nullptr;
    root->set_left(nullptr);
    root->set_right(nullptr);
    if (l) l->set_parent(nullptr);
    if (r) r->set_parent(nullptr);
    if (root->key < key) {
      split_impl(r, key, m, output_r);
      output_l = Derived::join3(l, root, m);
    } else {
      split_impl(l, key, output_l, m);
      output_r = Derived::join3(m, root, r);
    }
  }

 protected:
  NodesManagerType nodes_manager_;
};
}  // namespace base
}  // namespace bst
