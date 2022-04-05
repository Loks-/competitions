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
    size_t key = 0;
    size_t count = 0;
    size_t min_value = Empty;
    size_t max_value = Empty;
    TMask mask;

    bool IsEmpty() const { return count == 0; }
    bool IsSplit() const { return count > 1; }
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
        if (x.count > 0) {
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

  // Check
  void DeleteSubtree(Node *node, size_t x, size_t depth) {
    bool needRecursion = node->IsSplit();
    node->count = 0;
    --nodes_used;
    if (depth == 0 || !needRecursion) {
      return;
    }
    size_t highX = GetHighBits(x, depth);
    size_t idx;
    while (!node->mask.IsEmpty()) {
      idx = node->mask.MinI();
      node->mask.Delete(idx);
      size_t bitX = highX + (idx << (depth * bits_per_level));
      Node *subNode = FindNode(bitX, depth - 1);
      DeleteSubtree(subNode, bitX, depth - 1);
    }
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
      if (node->count == 0) return MakeSingleValueNode(node, x, depth);
      if (node->count == 1) {
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
    for (;;) {
      path[depth] = node;
      if (node->count == 1) {
        assert(!node->IsSplit());
        node->count = 0;
        assert(node->min_value == x);
        if (prev_node) {
          --nodes_used;
          assert(prev_node->mask.HasKey(prev_idx));
          prev_node->mask.Delete(prev_idx);
        }
        break;
      } else if (node->count == 2) {
        // unsplit node
        size_t x2 = node->max_value + node->min_value - x;
        DeleteSubtree(node, x, depth);
        ++nodes_used;  // Why ++?
        MakeSingleValueNode(node, x2, depth);
        break;
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
          break;
        } else {
          prev_node = node;
          node = FindNode(x, --depth);
        }
      }
    }
    for (++depth; depth <= max_depth; ++depth) {
      node = path[depth];
      if (node->min_value == x) {
        assert(node->max_value !=
               x);  // possible if multiple copies of same value allowed
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

  // Check
  size_t Successor(size_t x) {
    if (root.IsEmpty()) {
      return Empty;
    }
    Node *node = &root;
    size_t depth = max_depth;
    for (;;) {
      if (!node->IsSplit()) {
        assert(node->count == 1);
        if (node->min_value > x) {
          return node->min_value;
        } else {
          ++depth;
          break;
        }
      }
      path[depth] = node;
      size_t idx = CalcLevelIndex(x, depth);
      if (!node->mask.HasKey(idx)) {
        break;
      }
      if (depth == 0) {
        break;
      }
      --depth;
      node = FindNode(x, depth);
    }
    for (; depth <= max_depth; ++depth) {
      node = path[depth];
      size_t idx = CalcLevelIndex(x, depth);
      //   size_t minIdx;
      //   if (node->mask.GetLowestBitAfter(idx, &minIdx)) {
      size_t minIdx = node->mask.Successor(idx);
      if (minIdx != Empty) {
        size_t keyX =
            GetHighBits(x, depth) + (minIdx << (depth * bits_per_level));
        if (depth > 0) {
          const Node *nextNode = FindNode(keyX, depth - 1);
          return nextNode->min_value;
        } else {
          return keyX;
        }
      }
    }
    return Empty;
  }

  // Check
  size_t Predecessor(size_t x) {
    if (root.IsEmpty()) {
      return Empty;
    }
    Node *node = &root;
    size_t depth = max_depth;
    for (;;) {
      if (!node->IsSplit()) {
        assert(node->count == 1);
        if (node->min_value < x) {
          return node->min_value;
        } else {
          ++depth;
          break;
        }
      }
      size_t idx = CalcLevelIndex(x, depth);
      if (!node->mask.HasKey(idx)) {
        break;
      }
      if (depth == 0) {
        break;
      }
      --depth;
      node = FindNode(x, depth);
    }
    for (; depth <= max_depth; ++depth) {
      node = path[depth];
      size_t idx = CalcLevelIndex(x, depth);
      //   size_t maxIdx;
      //   if (node->mask.GetHighestBitBefore(idx, &maxIdx)) {
      size_t maxIdx = node->mask.Predecessor(idx);
      if (maxIdx != Empty) {
        size_t keyX =
            GetHighBits(x, depth) + (maxIdx << (depth * bits_per_level));
        if (depth > 0) {
          const Node *prev_node = FindNode(keyX, depth - 1);
          return prev_node->max_value;
        } else {
          return keyX;
        }
      }
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
