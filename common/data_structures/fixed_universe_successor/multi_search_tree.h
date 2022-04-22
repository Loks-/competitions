#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Based on A.G. code.
// Memory      -- O(S log U)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(log U)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log U)
// Predecessor -- O(log U)
// TODO: Move hash table code to a new class.
template <class TFLS>
class MultiSearchTree {
 protected:
  static const unsigned bits_per_level = TFLS::nbits;
  static const size_t level_mask = (size_t(1) << bits_per_level) - 1;

  struct Node {
    size_t key = 0;
    size_t min_value = Empty;
    size_t max_value = Empty;
    TFLS mask;

    bool IsEmpty() const { return min_value == Empty; }
    bool IsSplit() const { return min_value != max_value; }

    void Clear() {
      min_value = max_value = Empty;
      mask.Clear();
    }
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
    return GetHighBits(x, depth) + depth + 1;
  }

  size_t GetHash(size_t x) const {
    const size_t m = 0x4906ba494954cb65ull;
    return (x * m) >> hash_shift;
  }

  size_t Index(size_t x, size_t depth) const {
    return (x >> (depth * bits_per_level)) & level_mask;
  }

  void ResizeHash(size_t new_size) {
    size_t ln_size = 2;
    for (; (1ull << ln_size) < new_size * 2;) ++ln_size;
    hash_shift = 64 - ln_size;
    new_size = 1ull << ln_size;
    std::vector<Node> new_nodes;
    new_nodes.resize(new_size);
    node_ptr_mask = new_size - 1;
    if (nodes_used > 0) {
      for (const Node &x : nodes) {
        if (x.IsEmpty()) continue;
        for (size_t idx = GetHash(x.key);; ++idx) {
          size_t ptr = idx & node_ptr_mask;
          if (new_nodes[ptr].IsEmpty()) {
            new_nodes[ptr] = x;
            break;
          }
        }
      }
    }
    nodes.swap(new_nodes);
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

  void Set1(Node *node, size_t x, size_t depth) {
    node->min_value = node->max_value = x;
    node->mask.Set1I(Index(x, depth));
  }

 public:
  MultiSearchTree() { Init(64); }

  void Init(size_t u) {
    usize = u;
    root.Clear();
    max_depth = 0;
    for (;; ++max_depth) {
      size_t bits = (max_depth + 1) * bits_per_level;
      if ((bits >= 64) || ((1ull << bits) >= usize)) break;
    }
    path.resize(max_depth + 1);
    ResizeHash(1000);
  }

  size_t USize() const { return usize; }

  bool HasKey(size_t x) const {
    if (root.IsEmpty()) return false;
    const Node *node = &root;
    for (size_t depth = max_depth;; node = FindNode(x, --depth)) {
      if (!node->IsSplit()) return x == node->min_value;
      if (!node->mask.HasKey(Index(x, depth))) return false;
      if (depth == 0) return true;
    }
  }

  void Insert(size_t x) {
    if (nodes_used * 3 > node_ptr_mask) ResizeHash(4 * (node_ptr_mask + 1));
    Node *node = &root;
    for (size_t depth = max_depth;;) {
      if (node->IsEmpty()) return Set1(node, x, depth);
      // Split node if needed
      if (!node->IsSplit() && (depth > 0)) {
        if (node->min_value == x) return;  // Adding same value
        Set1(AddNode(node->min_value, depth - 1), node->min_value, depth - 1);
      }
      node->min_value = std::min(node->min_value, x);
      node->max_value = std::max(node->max_value, x);
      size_t idx = Index(x, depth);
      if (node->mask.HasKey(idx)) {
        if (depth == 0) return;
        node = FindNode(x, --depth);
      } else {
        node->mask.Insert(idx);
        if (depth == 0) return;
        node = AddNode(x, --depth);
      }
    }
  }

  void Delete(size_t x) {
    Node *prev_node = nullptr, *node = &root;
    size_t prev_idx = 0, depth = max_depth;
    for (;; node = FindNode(x, --depth)) {
      path[depth] = node;
      if (!node->IsSplit()) {
        if (node->min_value != x) return;  // Removing missing element
        node->Clear();
        if (prev_node) {
          --nodes_used;
          assert(prev_node->mask.HasKey(prev_idx));
          prev_node->mask.Delete(prev_idx);
        }
      } else {
        prev_idx = Index(x, depth);
        if (!node->mask.HasKey(prev_idx)) return;  // Removing missing element
        if (depth == 0) {
          node->mask.Delete(prev_idx);
          if (node->min_value == x) {
            node->min_value = x - prev_idx + node->mask.MinI();
          } else if (node->max_value == x) {
            node->max_value = x - prev_idx + node->mask.MaxI();
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
        auto x2 = GetHighBits(x, depth) +
                  (node->mask.MinI() << (depth * bits_per_level));
        node->min_value = FindNode(x2, depth - 1)->min_value;
      } else if (node->max_value == x) {
        auto x2 = GetHighBits(x, depth) +
                  (node->mask.MaxI() << (depth * bits_per_level));
        node->max_value = FindNode(x2, depth - 1)->max_value;
      } else {
        break;
      }
      if (node->min_value == node->max_value) {
        // Unsplit node
        Node *child = FindNode(node->min_value, depth - 1);
        assert(child);
        --nodes_used;
        child->Clear();
      }
    }
  }

  size_t Min() const { return root.min_value; }
  size_t Max() const { return root.max_value; }

  size_t Successor(size_t x) const {
    if (root.IsEmpty() || (x >= root.max_value)) return Empty;
    const Node *node = &root;
    for (size_t depth = max_depth;; --depth) {
      if (!node->IsSplit()) return node->max_value;
      size_t idx = Index(x, depth);
      if (depth == 0) return x - idx + node->mask.Successor(idx);
      if (node->mask.HasKey(idx)) {
        auto next_node = FindNode(x, depth - 1);
        if (x < next_node->max_value) {
          node = next_node;
          continue;
        }
      }
      auto idx2 = node->mask.Successor(idx);
      return FindNode(x + ((idx2 - idx) << (depth * bits_per_level)), depth - 1)
          ->min_value;
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    if (root.IsEmpty() || (x <= root.min_value)) return Empty;
    const Node *node = &root;
    for (size_t depth = max_depth;; --depth) {
      if (!node->IsSplit()) return node->min_value;
      size_t idx = Index(x, depth);
      if (depth == 0) return x - idx + node->mask.Predecessor(idx);
      if (node->mask.HasKey(idx)) {
        auto next_node = FindNode(x, depth - 1);
        if (x > next_node->min_value) {
          node = next_node;
          continue;
        }
      }
      auto idx2 = node->mask.Predecessor(idx);
      return FindNode(x - ((idx - idx2) << (depth * bits_per_level)), depth - 1)
          ->max_value;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
