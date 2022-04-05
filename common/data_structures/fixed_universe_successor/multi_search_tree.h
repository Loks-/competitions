#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Based on A.G. code.
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(log U)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(log U)
// Max         -- O(log U)
// Successor   -- O(log U)
// Predecessor -- O(log U)
template <class TMask>
class MultiSearchTree {
 protected:
  static const unsigned bits_per_level = TMask::nbits;
  static const size_t level_mask = (size_t(1) << bits_per_level) - 1;

  struct Node {
    size_t count = 0;
    size_t key = 0;
    size_t min_value = Empty;
    size_t max_value = Empty;
    TMask mask;

    bool IsEmpty() const { return min_value == Empty; }
    bool IsSplit() const { return min_value != max_value; }
  };

 protected:
  std::vector<Node> nodes;
  Node root;
  size_t usize;
  size_t hash_shift = 0;
  size_t node_ptr_mask = 0;
  size_t nodes_used = 0;
  size_t max_depth = 0;
  std::vector<Node *> path;

 protected:
  size_t GetHighBits(size_t x, size_t depth) const {
    if (depth >= max_depth) return 0;
    size_t mask_low_bits = (1ull << ((depth + 1) * bits_per_level)) - 1;
    return (x & ~mask_low_bits);
  }

  size_t GetHKey(size_t x, size_t depth) const {
    return GetHighBits(x, depth) + depth;
    // return GetHighBits(x, depth) + depth + 1;
  }

  size_t GetHash(size_t x) const {
    const size_t m = 0x4906ba494954cb65ull;
    return (x * m) >> hash_shift;
  }

  size_t CalcLevelIndex(size_t x, size_t depth) const {
    return (x >> (depth * bits_per_level)) & level_mask;
  }

  // Check
  void ResizeHash(size_t count) {
    // invalidates all pointers
    size_t nodeCountLn = 2;
    while ((1ull << nodeCountLn) < count * 2) {
      ++nodeCountLn;
    }
    hash_shift = 64 - nodeCountLn;
    size_t nodeCount = 1ull << nodeCountLn;
    std::vector<Node> newNodes;
    // ClearPodArray(&newNodes, nodeCount);
    newNodes.resize(nodeCount);
    node_ptr_mask = nodeCount - 1;
    if (nodes_used > 0) {
      for (const Node &x : nodes) {
        if (!x.IsEmpty()) {
          for (size_t idx = GetHash(x.key);; ++idx) {
            size_t ptr = idx & node_ptr_mask;
            if (newNodes[ptr].IsEmpty()) {
              newNodes[ptr] = x;
              break;
            }
          }
        }
      }
    }
    nodes.swap(newNodes);
  }

  size_t FindNodeIndex(size_t key) const {
    for (size_t idx = GetHash(key);; ++idx) {
      size_t ptr = idx & node_ptr_mask;
      if (nodes[ptr].key == key) {
        return ptr;
      }
    }
  }

  Node *FindNode(size_t x, size_t depth) {
    return &nodes[FindNodeIndex(GetHKey(x, depth))];
  }

  const Node *FindNode(size_t x, size_t depth) const {
    return &nodes[FindNodeIndex(GetHKey(x, depth))];
  }

  Node *AddNode(size_t x, size_t depth) {
    ++nodes_used;
    size_t key = GetHKey(x, depth);
    for (size_t idx = GetHash(key);; ++idx) {
      size_t ptr = idx & node_ptr_mask;
      if (nodes[ptr].IsEmpty()) {
        nodes[ptr].key = key;
        return &nodes[ptr];
      }
      assert(nodes[ptr].key != key);  // Key is already in hash table
    }
  }

  void DeleteSubtree(Node *node, size_t x, size_t depth) {
    bool needRecursion = node->IsSplit();
    node->min_value = node->max_value = Empty;  // Move to different function?
    node->count = 0;
    // node->key = 0;
    --nodes_used;
    if (depth == 0 || !needRecursion) return;
    size_t highX = GetHighBits(x, depth);
    for (size_t idx = node->mask.Min(); idx != Empty;
         idx = node->mask.Successor(idx)) {
      size_t bitX = highX + (idx << (depth * bits_per_level));
      Node *subNode = FindNode(bitX, depth - 1);
      DeleteSubtree(subNode, bitX, depth - 1);
    }
    // node->mask.Clear();
  }

  void MakeSingleValueNode(Node *node, size_t x, size_t depth) {
    node->count = 1;
    node->min_value = x;
    node->max_value = x;
    node->mask.Set1(CalcLevelIndex(x, depth));
  }

 public:
  MultiSearchTree() { Init(64); }

  void Init(size_t u) {
    usize = u;
    root.count = 0;
    root.min_value = Empty;
    root.max_value = Empty;
    root.mask.Clear();
    max_depth = 0;
    for (;; ++max_depth) {
      size_t bits = (max_depth + 1) * bits_per_level;
      if ((bits >= 64) || ((1ull << bits) >= usize)) break;
    }
    path.resize(max_depth + 1);
    ResizeHash(1000);  // ???
  }

  bool HasKey(size_t x) const {
    if (root.IsEmpty()) return false;
    const Node *node = &root;
    for (size_t depth = max_depth;; node = FindNode(x, --depth)) {
      if (!node->IsSplit()) return x == node->min_value;
      if (!node->mask.HasKey(CalcLevelIndex(x, depth))) return false;
      if (depth == 0) return true;
    }
  }

  // Adding same element twice will destroy internal counters.
  void Insert(size_t x) {
    // assert(!HasKey(x));
    if (nodes_used * 3 > node_ptr_mask) ResizeHash(4 * (node_ptr_mask + 1));
    Node *node = &root;
    for (size_t depth = max_depth;;) {
      if (node->IsEmpty()) return MakeSingleValueNode(node, x, depth);
      if (!node->IsSplit()) {
        // Split the node
        if (depth > 0) {
          size_t sx = node->min_value;
          Node *sn = AddNode(sx, depth - 1);
          MakeSingleValueNode(sn, sx, depth - 1);
        }
      }
      ++node->count;
      node->min_value = std::min(node->min_value, x);
      node->max_value = std::max(node->max_value, x);
      size_t idx = CalcLevelIndex(x, depth);
      if (node->mask.HasKey(idx)) {
        assert(depth);  // If depth == 0 we are adding same element.
        if (depth == 0) return;
        node = FindNode(x, --depth);
      } else {
        node->mask.Insert(idx);
        if (depth == 0) return;
        node = AddNode(x, --depth);
      }
    }
  }

  // Deleting element not in tree will destroy internal counters.
  void Delete(size_t x) {
    // assert(HasKey(x));
    Node *prev_node = nullptr;
    Node *node = &root;
    size_t prev_idx = 0;
    size_t depth = max_depth;
    for (;; node = FindNode(x, --depth)) {
      path[depth] = node;
      if (!node->IsSplit()) {
        node->count = 0;
        assert(node->min_value == x);
        node->min_value = node->max_value = Empty;
        if (prev_node) {
          --nodes_used;
          assert(prev_node->mask.HasKey(prev_idx));
          prev_node->mask.Delete(prev_idx);
        }
      } else if (node->count == 2) {
        // Unsplit node
        size_t x2 = node->max_value + node->min_value - x;
        DeleteSubtree(node, x, depth);
        ++nodes_used;
        MakeSingleValueNode(node, x2, depth);
      } else {
        --node->count;
        prev_idx = CalcLevelIndex(x, depth);
        assert(node->mask.HasKey(prev_idx));
        if (depth == 0) {
          node->mask.Delete(prev_idx);
          if (node->min_value == x) {
            node->min_value = GetHighBits(x, depth) + node->mask.MinI();
          } else if (node->max_value == x) {
            node->max_value = GetHighBits(x, depth) + node->mask.MaxI();
          }
        } else {
          prev_node = node;
          continue;
        }
      }
      break;
    }
    for (++depth; depth <= max_depth; ++depth) {
      node = path[depth];
      if (node->min_value == x) {
        size_t minIdx = node->mask.MinI();
        size_t keyX =
            GetHighBits(x, depth) + (minIdx << (depth * bits_per_level));
        Node *minNode = FindNode(keyX, depth - 1);
        node->min_value = minNode->min_value;
      } else if (node->max_value == x) {
        size_t maxIdx = node->mask.MaxI();
        size_t keyX =
            GetHighBits(x, depth) + (maxIdx << (depth * bits_per_level));
        Node *maxNode = FindNode(keyX, depth - 1);
        node->max_value = maxNode->max_value;
      } else {
        break;
      }
    }
  }

  size_t Min() const { return root.min_value; }
  size_t Max() const { return root.max_value; }

  size_t Successor(size_t x) {
    if (root.IsEmpty() || (x >= root.max_value)) return Empty;
    Node *node = &root, *next_node;
    for (size_t depth = max_depth;; --depth) {
      assert(x < node->max_value);
      if (!node->IsSplit()) {
        assert(node->count == 1);
        return node->max_value;
      }
      size_t idx = CalcLevelIndex(x, depth);
      if (depth == 0) return GetHighBits(x, depth) + node->mask.Successor(idx);
      bool stop = true;
      if (node->mask.HasKey(idx)) {
        next_node = FindNode(x, depth - 1);
        if (x < next_node->max_value) stop = false;
      }
      if (stop) {
        auto idx2 = node->mask.Successor(idx);
        assert(idx2 != Empty);
        size_t keyX =
            GetHighBits(x, depth) + (idx2 << (depth * bits_per_level));
        return FindNode(keyX, depth - 1)->min_value;
      }
      node = next_node;
    }
    return Empty;
  }

  size_t Predecessor(size_t x) {
    if (root.IsEmpty() || (x <= root.min_value)) return Empty;
    Node *node = &root, *next_node;
    for (size_t depth = max_depth;; --depth) {
      assert(x > node->min_value);
      if (!node->IsSplit()) {
        assert(node->count == 1);
        return node->min_value;
      }
      size_t idx = CalcLevelIndex(x, depth);
      if (depth == 0)
        return GetHighBits(x, depth) + node->mask.Predecessor(idx);
      bool stop = true;
      if (node->mask.HasKey(idx)) {
        next_node = FindNode(x, depth - 1);
        if (x > next_node->min_value) stop = false;
      }
      if (stop) {
        auto idx2 = node->mask.Predecessor(idx);
        assert(idx2 != Empty);
        size_t keyX =
            GetHighBits(x, depth) + (idx2 << (depth * bits_per_level));
        return FindNode(keyX, depth - 1)->max_value;
      }
      node = next_node;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
