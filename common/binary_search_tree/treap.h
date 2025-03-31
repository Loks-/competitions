#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/basic_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/treap_height.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <stack>
#include <tuple>
#include <vector>

namespace bst {

/**
 * @brief A Treap implementation of a binary search tree.
 *
 * A Treap is a binary search tree that maintains heap properties using random
 * priorities. This implementation provides efficient operations for insertion,
 * deletion, splitting, and joining trees.
 *
 * The treap maintains the following properties:
 * - Binary search tree property with respect to keys
 * - Heap property with respect to random priorities (heights)
 * - Efficient split and join operations
 * - Support for deferred operations
 *
 * @tparam use_key Whether the tree uses keys for ordering
 * @tparam use_parent Whether nodes maintain parent pointers
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering (if use_key is true)
 * @tparam NodesManager The node manager type for memory management
 */
template <bool use_key, bool use_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t,
          template <class> class NodesManager = memory::ContiguousNodesManager>
class Treap
    : public base::BasicTree<
          NodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependT<subtree_data::TreapHeight,
                                                    AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, use_parent, use_key, Key>>,
          Treap<use_key, use_parent, Data, AggregatorsTuple, DeferredTuple, Key,
                NodesManager>> {
 public:
  using SubtreeDataType = base::SubtreeData<
      templates::PrependT<subtree_data::TreapHeight, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, use_parent, use_key, Key>;
  using Self = Treap<use_key, use_parent, Data, AggregatorsTuple, DeferredTuple,
                     Key, NodesManager>;
  using Base = base::BasicTree<NodesManager<NodeType>, Self>;

  /**
   * @brief Operation support flags.
   *
   * These flags determine which operations are supported by the treap:
   * - support_insert: Whether insertion operations are supported
   * - support_remove: Whether removal operations are supported
   * - support_remove_by_node: Whether node-based removal is supported (requires
   * parent links)
   * - support_join: Whether two-way join operations are supported
   * - support_join3: Whether three-way join operations are supported
   * - support_split: Whether split operations are supported
   */
  static constexpr bool support_insert = true;
  static constexpr bool support_remove = true;
  static constexpr bool support_remove_by_node = Base::has_parent;
  static constexpr bool support_join = true;
  static constexpr bool support_join3 = true;
  static constexpr bool support_split = true;

 public:
  /**
   * @brief Constructs a treap with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr Treap(size_t max_nodes) : Base(max_nodes) {}

  /**
   * @brief Builds a treap from a vector of nodes.
   *
   * This implementation uses a stack-based approach to maintain the treap
   * properties while building the tree. It processes nodes in sequence and
   * maintains the heap property with respect to heights.
   *
   * @param nodes Vector of node pointers to build the treap from
   * @return Pointer to the root of the built treap
   */
  static constexpr NodeType* build_tree(const std::vector<NodeType*>& nodes) {
    if (nodes.empty()) return nullptr;
    NodeType* proot = nodes[0];
    assert(proot);
    NodeType* plast = proot;
    std::stack<NodeType*> s;
    for (size_t j = 1; j < nodes.size(); ++j) {
      NodeType* pj = nodes[j];
      assert(pj);
      if (height(pj) < height(plast)) {
        plast->set_right(pj);
        s.push(plast);
      } else if (height(pj) >= height(proot)) {
        for (plast->update_subtree_data(); !s.empty(); s.pop())
          s.top()->update_subtree_data();
        pj->set_left(proot);
        proot = pj;
      } else {
        for (plast->update_subtree_data(); height(pj) >= height(s.top());
             s.pop()) {
          plast = s.top();
          plast->update_subtree_data();
        }
        pj->set_left(plast);
        s.top()->set_right(pj);
      }
      plast = pj;
    }
    for (plast->update_subtree_data(); !s.empty(); s.pop())
      s.top()->update_subtree_data();
    return proot;
  }

  /**
   * @brief Joins two trees together.
   *
   * The join operation maintains both the binary search tree property
   * and the heap property with respect to heights.
   *
   * @param l The left tree
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join(NodeType* l, NodeType* r) {
    return !l ? r : !r ? l : join_impl(l, r);
  }

  /**
   * @brief Joins three trees together with a middle node.
   *
   * Note: This implementation is not optimal but is rarely used in practice.
   * It performs two join operations sequentially, which could be optimized
   * if this operation becomes a performance bottleneck.
   *
   * @param l The left tree
   * @param m1 The middle node
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join3(NodeType* l, NodeType* m1,
                                                 NodeType* r) {
    return join(l, join(m1, r));
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
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
    } else {
      split_impl(root, key, output_l, output_r);
      if (output_l) output_l->set_parent(nullptr);
      if (output_r) output_r->set_parent(nullptr);
    }
  }

  /**
   * @brief Splits a tree at a given inorder index.
   *
   * The split_at operation divides the tree into two parts based on the size:
   * - Left part contains the specified number of nodes in inorder traversal
   * - Right part contains the remaining nodes
   *
   * @param root The root of the tree to split
   * @param lsize The size of the left part
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split_at(NodeType* root, size_t lsize,
                                 NodeType*& output_l, NodeType*& output_r) {
    static_assert(SubtreeDataType::has_size,
                  "subtree data should contain size");
    if (!root) {
      output_l = output_r = nullptr;
    } else if (lsize == 0) {
      output_l = nullptr;
      output_r = root;
    } else if (lsize >= subtree_data::size(root)) {
      output_l = root;
      output_r = nullptr;
    } else {
      split_at_impl(root, lsize, output_l, output_r);
      if (output_l) output_l->set_parent(nullptr);
      if (output_r) output_r->set_parent(nullptr);
    }
  }

  /**
   * @brief Inserts a node into the tree.
   *
   * The insertion maintains both the binary search tree property
   * and the heap property with respect to heights. If the new node
   * has a higher priority (height), it becomes the new root.
   *
   * @param root The root of the tree to insert into
   * @param node The node to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* insert(NodeType* root,
                                                  NodeType* node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return node;
    root->apply_deferred();
    if (height(root) >= height(node)) {
      if (root->key < node->key) {
        root->set_right(insert(root->right, node));
      } else {
        root->set_left(insert(root->left, node));
      }
    } else {
      split_impl(root, node->key, node->left, node->right);
      if (node->left) node->left->set_parent(node);
      if (node->right) node->right->set_parent(node);
      root = node;
    }
    root->update_subtree_data();
    return root;
  }

  /**
   * @brief Inserts a node at the specified inorder index.
   *
   * This operation splits the tree at the given index and inserts
   * the new node between the resulting parts.
   *
   * @param root The root of the tree
   * @param node The node to insert
   * @param index The zero-based index where to insert
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* insert_at(NodeType* root,
                                                     NodeType* node,
                                                     size_t index) {
    static_assert(Base::has_size);
    assert(node);
    if (!root) {
      assert(index == 0);
      return node;
    } else {
      assert(index <= bst::subtree_data::size(root));
      NodeType *l = nullptr, *r = nullptr;
      split_at(root, index, l, r);
      return join3(l, node, r);
    }
  }

  /**
   * @brief Removes a specific node from the tree.
   *
   * This operation requires parent pointers to be enabled.
   * It joins the left and right subtrees of the removed node
   * and updates the parent's child pointer.
   *
   * @param node The node to remove
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_node(NodeType* node) {
    static_assert(Base::has_parent, "use_parent should be true");
    assert(node);
    return remove_node_impl(node);
  }

  /**
   * @brief Removes a node with the given key from the tree.
   *
   * The removal maintains both the binary search tree property
   * and the heap property with respect to heights. When a node
   * is removed, its subtrees are joined together.
   *
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove(NodeType* root,
                                                  const Key& key,
                                                  NodeType*& removed_node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return root;
    root->apply_deferred();
    if (root->key < key) {
      root->set_right(remove(root->right, key, removed_node));
    } else if (root->key > key) {
      root->set_left(remove(root->left, key, removed_node));
    } else {
      removed_node = root;
      NodeType* l = root->left;
      if (l) l->set_parent(nullptr);
      NodeType* r = root->right;
      if (r) r->set_parent(nullptr);
      root->reset_links_and_update_subtree_data();
      return join(l, r);
    }
    root->update_subtree_data();
    return root;
  }

  /**
   * @brief Merges two treaps into one.
   *
   * The merge operation combines two treaps while maintaining both
   * the binary search tree property and the heap property. It uses
   * the node with higher priority as the root and recursively
   * merges the remaining parts.
   *
   * @param p1 The first treap
   * @param p2 The second treap
   * @return Pointer to the root of the merged treap
   */
  [[nodiscard]] static constexpr NodeType* merge(NodeType* p1, NodeType* p2) {
    if (!p1) return p2;
    if (!p2) return p1;
    if (height(p1) < height(p2)) std::swap(p1, p2);
    NodeType *pt1 = nullptr, *pt2 = nullptr;
    p1->apply_deferred();
    split(p2, p1->key, pt1, pt2);
    p1->set_left(merge(p1->left, pt1));
    p1->set_right(merge(p1->right, pt2));
    p1->update_subtree_data();
    return p1;
  }

 protected:
  /**
   * @brief Gets the treap height (priority) of a node.
   *
   * The height value is used to maintain the heap property in the treap.
   * Higher values indicate higher priority in the heap.
   *
   * @param node The node to get the height for
   * @return The treap height of the node
   */
  [[nodiscard]] static constexpr unsigned height(const NodeType* node) {
    return subtree_data::TreapHeight::get(node);
  }

  /**
   * @brief Internal implementation of join operation.
   *
   * This implementation maintains both the binary search tree property
   * and the heap property by using the node with higher priority
   * as the root of the joined tree.
   *
   * @param l The left tree
   * @param r The right tree
   * @return Pointer to the new root of the joined tree
   */
  [[nodiscard]] static constexpr NodeType* join_impl(NodeType* l, NodeType* r) {
    if (height(l) > height(r)) {
      l->apply_deferred();
      l->set_right(l->right ? join_impl(l->right, r) : r);
      l->update_subtree_data();
      return l;
    } else {
      r->apply_deferred();
      r->set_left(r->left ? join_impl(l, r->left) : l);
      r->update_subtree_data();
      return r;
    }
  }

  /**
   * @brief Internal implementation of split operation by key.
   *
   * This implementation recursively splits the tree while maintaining
   * the binary search tree property and updating parent pointers.
   *
   * @param p The root node to split
   * @param key The key to split at
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split_impl(NodeType* p, const Key& key,
                                   NodeType*& output_l, NodeType*& output_r) {
    p->apply_deferred();
    if (p->key < key) {
      if (p->right) {
        output_l = p;
        split_impl(p->right, key, p->right, output_r);
        p->update_subtree_data();
        if (p->right) p->right->set_parent(p);
      } else {
        output_l = p;
        output_r = nullptr;
      }
    } else {
      if (p->left) {
        output_r = p;
        split_impl(p->left, key, output_l, p->left);
        p->update_subtree_data();
        if (p->left) p->left->set_parent(p);
      } else {
        output_l = nullptr;
        output_r = p;
      }
    }
  }

  /**
   * @brief Internal implementation of split operation by size.
   *
   * This implementation splits the tree at a given inorder index
   * while maintaining the binary search tree property and updating
   * parent pointers.
   *
   * @param p The root node to split
   * @param lsize The size of the left part
   * @param output_l Reference to store the left part
   * @param output_r Reference to store the right part
   */
  static constexpr void split_at_impl(NodeType* p, size_t lsize,
                                      NodeType*& output_l,
                                      NodeType*& output_r) {
    p->apply_deferred();
    const size_t hlsize = bst::subtree_data::size(p->left);
    if (lsize < hlsize) {
      output_r = p;
      split_at_impl(p->left, lsize, output_l, p->left);
      if (p->left) p->left->set_parent(p);
    } else if (lsize == hlsize) {
      output_l = p->left;
      output_r = p;
      p->left = nullptr;
    } else if (lsize == hlsize + 1) {
      output_l = p;
      output_r = p->right;
      p->right = nullptr;
    } else {
      output_l = p;
      split_at_impl(p->right, lsize - hlsize - 1, p->right, output_r);
      if (p->right) p->right->set_parent(p);
    }
    p->update_subtree_data();
  }

  /**
   * @brief Removes a specific node from the tree.
   *
   * This implementation handles the removal of a node by joining
   * its left and right subtrees and updating the parent's child
   * pointer. It maintains the treap properties throughout the
   * operation.
   *
   * @param node The node to remove
   * @return Pointer to the new root of the tree
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
    subtree_data::propagate_to_root(p);
    return base::root(p);
  }
};

}  // namespace bst
