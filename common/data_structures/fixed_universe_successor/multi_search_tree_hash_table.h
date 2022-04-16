#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/node.h"
#include "common/nodes_manager.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ds {
namespace fus {
// Similar to MultiSearchTree but with defaul hash table.
// Memory      -- O(S log U)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(log U)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(log U)
// Max         -- O(log U)
// Successor   -- O(log U)
// Predecessor -- O(log U)
template <class TFLS>
class MultiSearchTreeHashTable {
 protected:
  static const unsigned bits_per_level = TFLS::nbits;
  static const size_t level_mask = (size_t(1) << bits_per_level) - 1;

  class Node : public BaseNode {
   public:
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
  NodesManager<Node> nodes_manager;
  Node root;
  std::unordered_map<size_t, Node *> hash_table;
  size_t usize;
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

  size_t CalcLevelIndex(size_t x, size_t depth) const {
    return (x >> (depth * bits_per_level)) & level_mask;
  }

  Node *FindNode(size_t x, size_t depth) {
    auto key = GetHKey(x, depth);
    auto it = hash_table.find(key);
    return (it == hash_table.end()) ? nullptr : it->second;
  }

  const Node *FindNode(size_t x, size_t depth) const {
    auto key = GetHKey(x, depth);
    auto it = hash_table.find(key);
    return (it == hash_table.end()) ? nullptr : it->second;
  }

  void Set1(Node *node, size_t x, size_t depth) {
    node->min_value = node->max_value = x;
    node->mask.Set1(CalcLevelIndex(x, depth));
  }

  void AddNode(size_t x, size_t depth) {
    auto node = nodes_manager.New();
    hash_table[GetHKey(x, depth)] = node;
    Set1(node, x, depth);
  }

  void DeleteNode(size_t x, size_t depth) {
    auto key = GetHKey(x, depth);
    auto it = hash_table.find(key);
    if (it != hash_table.end()) {
      nodes_manager.Release(it->second);
      hash_table.erase(it);
    }
  }

 public:
  MultiSearchTreeHashTable() { Init(64); }

  void Init(size_t u) {
    usize = u;
    root.Clear();
    max_depth = 0;
    for (;; ++max_depth) {
      size_t bits = (max_depth + 1) * bits_per_level;
      if ((bits >= 64) || ((1ull << bits) >= usize)) break;
    }
    path.resize(max_depth + 1);
  }

  size_t USize() const { return usize; }

  bool HasKey(size_t x) const {
    if (root.IsEmpty()) return false;
    const Node *node = &root;
    for (size_t depth = max_depth;; node = FindNode(x, --depth)) {
      if (!node->IsSplit()) return x == node->min_value;
      if (!node->mask.HasKey(CalcLevelIndex(x, depth))) return false;
      if (depth == 0) return true;
    }
  }

  void Insert(size_t x) {
    if (root.IsEmpty()) return Set1(&root, x, max_depth);
    Node *node = &root;
    for (size_t depth = max_depth;;) {
      // Split node if needed
      if (!node->IsSplit() && (depth > 0)) {
        if (node->min_value == x) return;  // Adding same value
        AddNode(node->min_value, depth - 1);
      }
      node->min_value = std::min(node->min_value, x);
      node->max_value = std::max(node->max_value, x);
      size_t idx = CalcLevelIndex(x, depth);
      if (node->mask.HasKey(idx)) {
        if (depth == 0) return;
        node = FindNode(x, --depth);
      } else {
        node->mask.Insert(idx);
        if (depth == 0) return;
        return AddNode(x, depth - 1);
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
        DeleteNode(x, depth);
        if (prev_node) {
          assert(prev_node->mask.HasKey(prev_idx));
          prev_node->mask.Delete(prev_idx);
        }
      } else {
        prev_idx = CalcLevelIndex(x, depth);
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
      if (node->min_value == node->max_value)
        DeleteNode(node->min_value, depth - 1);
    }
  }

  size_t Min() const { return root.min_value; }
  size_t Max() const { return root.max_value; }

  size_t Successor(size_t x) const {
    if (root.IsEmpty() || (x >= root.max_value)) return Empty;
    const Node *node = &root;
    for (size_t depth = max_depth;; --depth) {
      if (!node->IsSplit()) return node->max_value;
      size_t idx = CalcLevelIndex(x, depth);
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
      size_t idx = CalcLevelIndex(x, depth);
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
