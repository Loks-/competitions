#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/base/base_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/right.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/splay.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

namespace bst {

/**
 * @brief A SplayTree implementation of a binary search tree.
 *
 * A SplayTree is a self-adjusting binary search tree that uses splay operations
 * to maintain amortized balance. Unlike traditional balanced trees, it doesn't
 * maintain explicit balance information but achieves amortized O(log n)
 * performance through splay operations.
 *
 * The splay tree maintains the following properties:
 * - Binary search tree property with respect to keys
 * - Self-adjusting: Automatically reorganizes based on access patterns
 * - Amortized O(log n) operations
 * - Support for deferred operations
 * - Support for subtree data aggregation
 *
 * @note Search and access functions (find, at, lower_bound, etc.) have a
 *       different interface compared to other BST implementations because they
 * modify the tree structure by splaying the accessed node to the root. This
 * means:
 *       - The root node of the tree may change after these operations
 *       - These functions take a reference to the root pointer to update it
 *       - The caller should not assume the root remains the same after these
 * operations
 *
 * @tparam has_key Whether the tree uses keys for ordering
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering (if has_key is true)
 * @tparam NodesManager The node manager type for memory management
 */
template <bool has_key, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t,
          template <class> class NodesManager = memory::ContiguousNodesManager>
class SplayTree
    : public base::BaseTree<
          NodesManager<
              base::Node<Data, base::SubtreeData<AggregatorsTuple>,
                         base::Deferred<DeferredTuple>, true, has_key, Key>>,
          SplayTree<has_key, Data, AggregatorsTuple, DeferredTuple, Key,
                    NodesManager>> {
 public:
  using SubtreeDataType = base::SubtreeData<AggregatorsTuple>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, true, has_key, Key>;
  using Self = SplayTree<has_key, Data, AggregatorsTuple, DeferredTuple, Key,
                         NodesManager>;
  using Base = base::BaseTree<NodesManager<NodeType>, Self>;

  friend Base;

 public:
  /**
   * @brief Constructs a splay tree with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr SplayTree(size_t max_nodes)
      : Base(max_nodes) {}

  /**
   * @brief Default constructor for SplayTree.
   *
   * This constructor creates a splay tree with no reserved nodes.
   */
  [[nodiscard]] constexpr SplayTree() : SplayTree(0) {}

  /**
   * @brief Finds a node with a given key in the tree.
   *
   * The find operation searches for a node with the given key and splays the
   * last accessed node to the root. This means the root of the tree will change
   * after this operation.
   *
   * @param root The root of the tree (will be updated to point to the splayed
   * node)
   * @param key The key to search for
   * @return The node with the given key, or nullptr if not found
   */
  [[nodiscard]] static constexpr NodeType* find(NodeType*& root,
                                                const Key& key) {
    static_assert(has_key, "has_key should be true");
    NodeType* node = root;
    for (; node;) {
      root = node;
      node->apply_deferred();
      if (node->key < key) {
        node = node->right;
      } else if (key < node->key) {
        node = node->left;
      } else {
        break;
      }
    }
    base::splay(root);
    return node;
  }

  /**
   * @brief Finds the largest node with a key less than or equal to the given
   * key.
   *
   * The floor operation finds the largest node with a key less than or equal to
   * the given key and splays it to the root. This means the root of the tree
   * will change after this operation.
   *
   * @param root The root of the tree (will be updated to point to the splayed
   * node)
   * @param key The key to search for
   * @return The largest node with a key less than or equal to the given key
   */
  [[nodiscard]] static constexpr NodeType* floor(NodeType*& root,
                                                 const Key& key) {
    static_assert(has_key, "has_key should be true");
    NodeType* node_floor = nullptr;
    for (NodeType* node = root; node;) {
      root = node;
      node->apply_deferred();
      if (node->key <= key) {
        node_floor = node;
        node = node->right;
      } else {
        node = node->left;
      }
    }
    base::splay(root);
    return node_floor;
  }

  /**
   * @brief Finds the smallest node with a key greater than or equal to the
   * given key.
   *
   * The lower_bound operation finds the smallest node with a key greater than
   * or equal to the given key and splays it to the root. This means the root of
   * the tree will change after this operation.
   *
   * @param root The root of the tree (will be updated to point to the splayed
   * node)
   * @param key The key to search for
   * @return The smallest node with a key greater than or equal to the given key
   */
  [[nodiscard]] static constexpr NodeType* lower_bound(NodeType*& root,
                                                       const Key& key) {
    static_assert(has_key, "has_key should be true");
    NodeType* node_ceil = nullptr;
    for (NodeType* node = root; node;) {
      root = node;
      node->apply_deferred();
      if (node->key < key) {
        node = node->right;
      } else {
        node_ceil = node;
        node = node->left;
      }
    }
    base::splay(root);
    return node_ceil;
  }

  /**
   * @brief Finds the node at the given index in the tree.
   *
   * The at operation finds the node at the given index and splays it to the
   * root. This means the root of the tree will change after this operation.
   *
   * @param root The root of the tree (will be updated to point to the splayed
   * node)
   * @param order_index The index to search for
   * @return The node at the given index
   */
  [[nodiscard]] static constexpr NodeType* at(NodeType*& root,
                                              size_t order_index) {
    static_assert(SubtreeDataType::has_size, "info should contain size");
    auto node = base::at(root, order_index);
    base::splay(node);
    if (node) root = node;
    return node;
  }

  /**
   * @brief Gets the inorder index of a node.
   *
   * The index operation returns the position of a node in the inorder traversal
   * of the tree.
   *
   * @param node The node to get the index of
   * @return The inorder index of the node
   */
  [[nodiscard]] static constexpr size_t index(NodeType* node) {
    assert(node);
    base::splay(node);
    return subtree_data::size(node->left);
  }

  /**
   * @brief Removes a node with the given key from the tree.
   *
   * The remove operation finds and removes a node with the specified key while
   * maintaining the binary search tree property.
   *
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @param removed_node Reference to store the removed node
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* remove(NodeType* root,
                                                  const Key& key,
                                                  NodeType*& removed_node) {
    static_assert(has_key, "has_key should be true");
    removed_node = find(root, key);
    return (removed_node ? remove_node_impl(removed_node) : root);
  }

  /**
   * @brief Removes a node at the given index from the tree.
   *
   * The remove_at operation removes a node at the specified position in the
   * inorder traversal of the tree.
   *
   * @param root The root of the tree
   * @param index The position of the node to remove
   * @param removed_node Reference to store the removed node
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* remove_at(NodeType* root,
                                                     size_t index,
                                                     NodeType*& removed_node) {
    static_assert(Base::has_size, "has_size should be true");
    removed_node = base::at(root, index);
    return (removed_node ? remove_node_impl(removed_node) : root);
  }

  /**
   * @brief Removes a specific node from the tree.
   *
   * The remove_node operation removes the given node while maintaining the
   * binary search tree property.
   *
   * @param node The node to remove
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* remove_node(NodeType* node) {
    assert(node);
    return remove_node_impl(node);
  }

  /**
   * @brief Joins two trees into a single tree.
   *
   * The join operation combines two trees by finding the rightmost node in the
   * left tree, splaying it to the root, and attaching the right tree as its
   * right child.
   *
   * @param l The root of the left tree
   * @param r The root of the right tree
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* join(NodeType* l, NodeType* r) {
    if (!l) return r;
    if (!r) return l;
    assert(!l->parent && !r->parent);
    NodeType* p = base::right(l);
    p->set_right(r);
    base::splay(p);
    return p;
  }

  /**
   * @brief Joins three trees into a single tree.
   *
   * The join3 operation combines three trees by using the middle node as the
   * root and attaching the left and right trees as its children.
   *
   * @param l The root of the left tree
   * @param m1 The middle node (must be a leaf)
   * @param r The root of the right tree
   * @return The root of the resulting tree (m1)
   */
  static constexpr NodeType* join3(NodeType* l, NodeType* m1, NodeType* r) {
    assert(m1 && !m1->left && !m1->right);
    m1->set_left(l);
    m1->set_right(r);
    m1->update_subtree_data();
    return m1;
  }

  /**
   * @brief Splits a tree into two trees at a given node.
   *
   * The split operation divides the tree into two parts: everything to the
   * left of the given node and the node itself with everything to its right.
   *
   * @param p The node at which to split
   * @return The root of the left tree
   */
  [[nodiscard]] static constexpr NodeType* split_left(NodeType* p) {
    if (!p) return nullptr;
    base::splay(p);
    NodeType* l = p->left;
    if (l) {
      l->parent = nullptr;
      p->left = nullptr;
      p->update_subtree_data();
    }
    return l;
  }

  /**
   * @brief Splits a tree into two trees at a given node.
   *
   * The split operation divides the tree into two parts: the node itself with
   * everything to its left and everything to its right.
   *
   * @param p The node at which to split
   * @return The root of the right tree
   */
  [[nodiscard]] static constexpr NodeType* split_right(NodeType* p) {
    if (!p) return nullptr;
    base::splay(p);
    NodeType* r = p->right;
    if (r) {
      r->parent = nullptr;
      p->right = nullptr;
      p->update_subtree_data();
    }
    return r;
  }

  /**
   * @brief Splits a tree at a given key.
   *
   * The split operation divides the tree into two parts based on the key value:
   * - Left part contains all nodes with keys less than the given key
   * - Right part contains all nodes with keys greater than or equal to the key
   *
   * @param root The root of the tree to split
   * @param key The key to split at
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split(NodeType* root, const Key& key,
                              NodeType*& output_l, NodeType*& output_r) {
    static_assert(has_key, "has_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
      return;
    }
    NodeType* p = lower_bound(root, key);
    output_r = p;
    output_l = (p ? split_left(p) : root);
  }

  /**
   * @brief Splits a tree at a given index.
   *
   * The split_at operation divides the tree into two parts based on the given
   * size: the first lsize nodes in inorder traversal go to the left tree, and
   * the remaining nodes go to the right tree.
   *
   * @param root The root of the tree to split
   * @param lsize The size of the left tree
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split_at(NodeType* root, size_t lsize,
                                 NodeType*& output_l, NodeType*& output_r) {
    static_assert(SubtreeDataType::has_size, "info should contain size");
    if (!root) {
      output_l = output_r = nullptr;
    } else if (lsize == 0) {
      output_l = nullptr;
      output_r = root;
    } else if (lsize >= subtree_data::size(root)) {
      output_l = root;
      output_r = nullptr;
    } else {
      NodeType* p = at(root, lsize);
      output_l = split_left(p);
      output_r = p;
    }
  }

  /**
   * @brief Merges two trees into a single tree.
   *
   * The merge operation combines two trees by recursively splitting the smaller
   * tree around the median of the larger tree.
   *
   * @param root1 The root of the first tree
   * @param root2 The root of the second tree
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* merge(NodeType* root1,
                                                 NodeType* root2) {
    static_assert(has_key, "has_key should be true");
    static_assert(SubtreeDataType::has_size, "info should contain size");
    if (!root1) return root2;
    if (!root2) return root1;

    // Verify that both nodes are actually roots of their trees
    assert(root1->is_root());
    assert(root2->is_root());

    if (subtree_data::size(root1) < subtree_data::size(root2))
      std::swap(root1, root2);
    NodeType* m = at(root1, subtree_data::size(root1) / 2);
    NodeType *r2l = nullptr, *r2r = nullptr;
    split(root2, m->key, r2l, r2r);
    if (m->left) m->left->parent = nullptr;
    if (m->right) m->right->parent = nullptr;
    m->set_left(merge(m->left, r2l));
    m->set_right(merge(m->right, r2r));
    m->update_subtree_data();
    return m;
  }

 protected:
  /**
   * @brief Implementation of node insertion into the tree.
   *
   * This function implements the insertion of a node into the splay tree while
   * maintaining the binary search tree property. The base class handles all
   * requirements checking. After insertion, the inserted node is splayed to the
   * root to maintain the splay tree property.
   *
   * The insertion process:
   * 1. The node is inserted into the appropriate position based on its key
   * 2. The node is splayed to the root to maintain the splay tree property
   * 3. The tree structure is updated to reflect the changes
   *
   * @tparam update_required Whether to update subtree data for the inserted
   * node. This parameter is only active when the new node is added as a leaf.
   *        If the new node becomes a root or internal node, subtree data is
   *        always updated.
   * @param root The root of the tree
   * @param node The node to insert
   * @return Pointer to the new root of the tree (the inserted node)
   */
  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_impl(NodeType* root,
                                                       NodeType* node) {
    if (!root) {
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }

    while (true) {
      root->apply_deferred();
      if (root->key < node->key) {
        if (root->right) {
          root = root->right;
        } else {
          root->set_right(node);
          break;
        }
      } else {
        if (root->left) {
          root = root->left;
        } else {
          root->set_left(node);
          break;
        }
      }
    }
    base::splay(node);
    return node;
  }

  /**
   * @brief Implementation of node insertion at a specific index.
   *
   * This function implements the insertion of a node at the given position in
   * the inorder traversal of the splay tree while maintaining the binary search
   * tree property. The base class handles all requirements checking. After
   * insertion, the inserted node is splayed to the root to maintain the splay
   * tree property.
   *
   * The insertion process:
   * 1. The node is inserted at the specified index position
   * 2. The node is splayed to the root to maintain the splay tree property
   * 3. The tree structure is updated to reflect the changes
   *
   * @tparam update_required Whether to update subtree data for the inserted
   * node. This parameter is only active when the new node is added as a leaf.
   *        If the new node becomes a root or internal node, subtree data is
   *        always updated.
   * @param root The root of the tree
   * @param node The node to insert
   * @param index The zero-based index where to insert
   * @return Pointer to the new root of the tree (the inserted node)
   */
  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_at_impl(NodeType* root,
                                                          NodeType* node,
                                                          size_t index) {
    if (!root) {
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }

    while (true) {
      root->apply_deferred();
      if (index <= subtree_data::size(root->left)) {
        if (root->left) {
          root = root->left;
        } else {
          root->set_left(node);
          break;
        }
      } else {
        index -= subtree_data::size(root->left) + 1;
        if (root->right) {
          root = root->right;
        } else {
          root->set_right(node);
          break;
        }
      }
    }
    assert(index == 0);
    base::splay(node);
    return node;
  }

  /**
   * @brief Removes a node from the tree.
   *
   * The remove_node_impl operation removes a node from the tree by joining its
   * left and right subtrees and updating the parent's child pointer.
   *
   * @param node The node to remove
   * @return The root of the resulting tree
   */
  [[nodiscard]] static constexpr NodeType* remove_node_impl(NodeType* node) {
    NodeType* l = node->left;
    if (l) l->set_parent(nullptr);
    NodeType* r = node->right;
    if (r) r->set_parent(nullptr);
    NodeType* p = node->parent;
    node->reset_links_and_update_subtree_data();
    NodeType* m = join(l, r);
    if (!p) return m;
    if (node == p->left) {
      p->set_left(m);
    } else {
      p->set_right(m);
    }
    base::splay(p);
    return p;
  }
};

}  // namespace bst
