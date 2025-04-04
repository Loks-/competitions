#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/base_tree.h"
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
 * @tparam has_key Whether the tree uses keys for ordering
 * @tparam has_parent Whether nodes maintain parent pointers
 * @tparam Data The data type stored in each node
 * @tparam AggregatorsTuple Tuple of aggregator types for subtree data
 * @tparam DeferredTuple Tuple of deferred operation types
 * @tparam Key The key type used for ordering (if has_key is true)
 * @tparam NodesManager The node manager type for memory management
 */
template <bool has_key, bool has_parent, typename Data,
          typename AggregatorsTuple = std::tuple<subtree_data::Size>,
          typename DeferredTuple = std::tuple<>, typename Key = int64_t,
          template <class> class NodesManager = memory::ContiguousNodesManager>
class Treap
    : public base::BaseTree<
          NodesManager<base::Node<
              Data,
              base::SubtreeData<templates::PrependT<subtree_data::TreapHeight,
                                                    AggregatorsTuple>>,
              base::Deferred<DeferredTuple>, has_parent, has_key, Key>>,
          Treap<has_key, has_parent, Data, AggregatorsTuple, DeferredTuple, Key,
                NodesManager>> {
 public:
  using SubtreeDataType = base::SubtreeData<
      templates::PrependT<subtree_data::TreapHeight, AggregatorsTuple>>;
  using DeferredType = base::Deferred<DeferredTuple>;
  using NodeType =
      base::Node<Data, SubtreeDataType, DeferredType, has_parent, has_key, Key>;
  using Self = Treap<has_key, has_parent, Data, AggregatorsTuple, DeferredTuple,
                     Key, NodesManager>;
  using Base = base::BaseTree<NodesManager<NodeType>, Self>;

  friend Base;

 public:
  /**
   * @brief Constructs a treap with the specified maximum number of nodes.
   *
   * @param max_nodes The maximum number of nodes to reserve
   */
  [[nodiscard]] explicit constexpr Treap(size_t max_nodes) : Base(max_nodes) {}

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
    static_assert(has_key, "has_key should be true");
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
   * @brief Implementation of node insertion into the tree.
   *
   * This function implements the insertion of a node into the treap while
   * maintaining both the binary search tree property and the heap property
   * with respect to heights. The base class handles all requirements checking.
   *
   * The insertion process:
   * 1. If the new node has higher priority (height) than the root, it becomes
   *    the new root
   * 2. Otherwise, it is inserted into the appropriate subtree based on its key
   * 3. The heap property is maintained by rotating the node up if needed
   *
   * @tparam update_required Whether to update subtree data for the inserted
   * node. This parameter is only active when the new node is added as a leaf.
   *        If the new node becomes a root or internal node, subtree data is
   *        always updated.
   * @param root The root of the tree
   * @param node The node to insert
   * @return Pointer to the new root of the tree
   */
  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_impl(NodeType* root,
                                                       NodeType* node) {
    if (!root) {
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }

    root->apply_deferred();
    if (height(root) >= height(node)) {
      if (root->key < node->key) {
        root->set_right(insert_impl<update_required>(root->right, node));
      } else {
        root->set_left(insert_impl<update_required>(root->left, node));
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
   * @brief Implementation of node insertion at a specific index.
   *
   * This function implements the insertion of a node at the given position in
   * the inorder traversal of the treap while maintaining both the binary search
   * tree property and the heap property with respect to heights. The base class
   * handles all requirements checking.
   *
   * The insertion process:
   * 1. If the new node has higher priority (height) than the root, it becomes
   *    the new root
   * 2. Otherwise, it is inserted into the appropriate subtree based on its
   * index
   * 3. The heap property is maintained by rotating the node up if needed
   *
   * @tparam update_required Whether to update subtree data for the inserted
   * node. This parameter is only active when the new node is added as a leaf.
   *        If the new node becomes a root or internal node, subtree data is
   *        always updated.
   * @param root The root of the tree
   * @param node The node to insert
   * @param index The zero-based index where to insert
   * @return Pointer to the new root of the tree
   */
  template <bool update_required>
  [[nodiscard]] static constexpr NodeType* insert_at_impl(NodeType* root,
                                                          NodeType* node,
                                                          size_t index) {
    if (!root) {
      if constexpr (update_required) node->update_subtree_data();
      return node;
    }
    assert(index <= subtree_data::size(root));

    root->apply_deferred();
    if (height(root) >= height(node)) {
      const size_t left_size = subtree_data::size(root->left);
      if (index <= left_size) {
        root->set_left(
            insert_at_impl<update_required>(root->left, node, index));
      } else {
        root->set_right(insert_at_impl<update_required>(root->right, node,
                                                        index - left_size - 1));
      }
    } else {
      split_at_impl(root, index, node->left, node->right);
      if (node->left) node->left->set_parent(node);
      if (node->right) node->right->set_parent(node);
      root = node;
    }
    root->update_subtree_data();
    return root;
  }

  /**
   * @brief Implementation of node removal from the tree by key.
   *
   * This function implements the removal of a node with the specified key from
   * the treap while maintaining both the binary search tree property and the
   * heap property with respect to heights. The base class handles all
   * requirements checking.
   *
   * The removal process:
   * 1. The node is removed from its position in the tree
   * 2. Its left and right subtrees are joined together
   * 3. The heap property is maintained throughout the operation
   *
   * @tparam reset_links Whether to reset links of the removed node. This is
   *         useful when the node will be reused, but can be skipped if the
   *         node is going to be released.
   * @param root The root of the tree
   * @param key The key of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_impl(
      NodeType* root, const Key& key, NodeType*& removed_node) {
    if (!root) return root;
    root->apply_deferred();
    if (root->key < key) {
      root->set_right(remove_impl<reset_links>(root->right, key, removed_node));
      root->update_subtree_data();
    } else if (root->key > key) {
      root->set_left(remove_impl<reset_links>(root->left, key, removed_node));
      root->update_subtree_data();
    } else {
      removed_node = root;
      root = join(root->left, root->right);
      if (root) root->set_parent(nullptr);
      if constexpr (reset_links)
        removed_node->reset_links_and_update_subtree_data();
    }
    return root;
  }

  /**
   * @brief Implementation of node removal from the tree by index.
   *
   * This function implements the removal of a node at the specified position
   * in the inorder traversal of the treap while maintaining both the binary
   * search tree property and the heap property with respect to heights. The
   * base class handles all requirements checking.
   *
   * The removal process:
   * 1. The node is removed from its position in the tree
   * 2. Its left and right subtrees are joined together
   * 3. The heap property is maintained throughout the operation
   *
   * @tparam reset_links Whether to reset links of the removed node. This is
   *         useful when the node will be reused, but can be skipped if the
   *         node is going to be released.
   * @param root The root of the tree
   * @param index The position of the node to remove
   * @param removed_node Reference to store the removed node
   * @return Pointer to the new root of the tree
   */
  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_at_impl(
      NodeType* root, size_t index, NodeType*& removed_node) {
    if (!root) return root;
    root->apply_deferred();
    const size_t left_size = subtree_data::size(root->left);
    if (index < left_size) {
      root->set_left(
          remove_at_impl<reset_links>(root->left, index, removed_node));
      root->update_subtree_data();
    } else if (index == left_size) {
      removed_node = root;
      root = join(root->left, root->right);
      if (root) root->set_parent(nullptr);
      if constexpr (reset_links)
        removed_node->reset_links_and_update_subtree_data();
    } else {
      root->set_right(remove_at_impl<reset_links>(
          root->right, index - left_size - 1, removed_node));
      root->update_subtree_data();
    }
    return root;
  }

  /**
   * @brief Implementation of specific node removal from the tree.
   *
   * This function implements the removal of a specific node from the treap
   * while maintaining both the binary search tree property and the heap
   * property with respect to heights. The base class handles all requirements
   * checking.
   *
   * The removal process:
   * 1. The node is removed from its position in the tree
   * 2. Its left and right subtrees are joined together
   * 3. The heap property is maintained throughout the operation
   *
   * @tparam reset_links Whether to reset links of the removed node. This is
   *         useful when the node will be reused, but can be skipped if the
   *         node is going to be released.
   * @param node The node to remove
   * @return Pointer to the new root of the tree
   */
  template <bool reset_links>
  [[nodiscard]] static constexpr NodeType* remove_node_impl(NodeType* node) {
    NodeType *p = node->parent, *m = join(node->left, node->right);
    if constexpr (reset_links) node->reset_links_and_update_subtree_data();
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
