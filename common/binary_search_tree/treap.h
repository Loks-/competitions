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

  friend Base;

 public:
  /**
   * @brief Constructs a treap with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr Treap(size_t max_nodes) : Base(max_nodes) {}

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
   * This operation maintains both the binary search tree property
   * and the heap property with respect to heights. It uses a similar
   * approach to insert but based on inorder position rather than key.
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
    }
    assert(index <= subtree_data::size(root));

    root->apply_deferred();
    if (height(root) >= height(node)) {
      const size_t left_size = subtree_data::size(root->left);
      if (index <= left_size) {
        root->set_left(insert_at(root->left, node, index));
      } else {
        root->set_right(insert_at(root->right, node, index - left_size - 1));
      }
    } else {
      NodeType *l = nullptr, *r = nullptr;
      split_at_impl(root, index, l, r);
      node->set_left(l);
      node->set_right(r);
      root = node;
    }
    root->update_subtree_data();
    return root;
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
      root->update_subtree_data();
    } else if (root->key > key) {
      root->set_left(remove(root->left, key, removed_node));
      root->update_subtree_data();
    } else {
      removed_node = root;
      root = join(root->left, root->right);
      if (root) root->set_parent(nullptr);
      removed_node->reset_links_and_update_subtree_data();
    }
    return root;
  }

  /**
   * @brief Removes a node at the given index from the tree.
   *
   * The remove_at operation removes a node at the specified position in the
   * inorder traversal of the tree. It maintains both the binary search tree
   * property and the heap property with respect to heights.
   *
   * @param root The root of the tree
   * @param index The position of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  [[nodiscard]] static constexpr NodeType* remove_at(NodeType* root,
                                                     size_t index,
                                                     NodeType*& removed_node) {
    static_assert(Base::has_size, "has_size should be true");
    if (!root) return root;
    root->apply_deferred();
    const size_t left_size = subtree_data::size(root->left);
    if (index < left_size) {
      root->set_left(remove_at(root->left, index, removed_node));
      root->update_subtree_data();
    } else if (index == left_size) {
      removed_node = root;
      root = join(root->left, root->right);
      if (root) root->set_parent(nullptr);
      removed_node->reset_links_and_update_subtree_data();
    } else {
      root->set_right(
          remove_at(root->right, index - left_size - 1, removed_node));
      root->update_subtree_data();
    }
    return root;
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
    p1->apply_deferred();
    NodeType *pt1 = nullptr, *pt2 = nullptr;
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

  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_base_impl(
      const std::vector<NodeType*>& nodes, size_t begin, size_t end) = delete;

  /**
   * @brief Builds a treap from a vector of nodes.
   *
   * This implementation uses a stack-based approach to maintain the treap
   * properties while building the tree. It processes nodes in sequence and
   * maintains the heap property with respect to heights. The template parameter
   * update_leafs is not used as subtree data is always updated to maintain
   * correct treap properties.
   *
   * The algorithm works as follows:
   * 1. Processes nodes in sequence, maintaining a stack of nodes
   * 2. For each new node:
   *    - If it has lower priority than the last node, becomes its right child
   *    - If it has higher priority than the root, becomes the new root
   *    - Otherwise, finds the right position in the stack to maintain heap
   * property
   * 3. Updates subtree data for all affected nodes
   *
   * @tparam update_leafs Unused parameter (kept for interface compatibility)
   * @param nodes Vector of node pointers to build the treap from
   * @return Pointer to the root of the built treap
   */
  template <bool update_leafs>
  [[nodiscard]] static constexpr NodeType* build_tree_impl(
      const std::vector<NodeType*>& nodes) {
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
    NodeType *p = node->parent, *m = join(node->left, node->right);
    node->reset_links_and_update_subtree_data();
    if (!p) {
      if (m) m->set_parent(nullptr);
      return m;
    } else if (node == p->left) {
      p->set_left(m);
    } else {
      p->set_right(m);
    }
    subtree_data::propagate_to_root(p);
    return base::root(p);
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
    const size_t hlsize = subtree_data::size(p->left);
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
};

}  // namespace bst
