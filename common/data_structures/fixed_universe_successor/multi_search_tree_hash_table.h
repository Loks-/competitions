#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace ds {
namespace fus {
// Similar to MultiSearchTree but with default hash table.
// Memory      -- O(S log U)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(log log U)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
template <class TFLS>
class MultiSearchTreeHashTable {
 protected:
  static constexpr unsigned bits_per_level = TFLS::nbits;
  static constexpr size_t level_mask = (size_t(1) << bits_per_level) - 1;

  class Node : public memory::Node {
   public:
    size_t min_value = kEmpty;
    size_t max_value = kEmpty;
    TFLS mask;

   public:
    constexpr bool IsEmpty() const { return min_value == kEmpty; }

    constexpr bool IsSplit() const { return min_value != max_value; }

    constexpr void Clear() {
      min_value = max_value = kEmpty;
      mask.Clear();
    }
  };

 protected:
  memory::NodesManager<Node> nodes_manager;
  Node root;
  std::unordered_map<size_t, Node *> hash_table;
  size_t usize;
  size_t maxh = 0;
  std::vector<Node *> path;

 protected:
  size_t HBits(size_t x, size_t h) const {
    if (h >= maxh) return 0;
    const size_t mask_low_bits = (1ull << ((h + 1) * bits_per_level)) - 1;
    return (x & ~mask_low_bits);
  }

  size_t HKey(size_t x, size_t h) const { return HBits(x, h) + h + 1; }

  size_t Index(size_t x, size_t h) const {
    return (x >> (h * bits_per_level)) & level_mask;
  }

  Node *FindNode(size_t x, size_t h) {
    auto key = HKey(x, h);
    auto it = hash_table.find(key);
    return (it == hash_table.end()) ? nullptr : it->second;
  }

  const Node *FindNode(size_t x, size_t h) const {
    auto key = HKey(x, h);
    auto it = hash_table.find(key);
    return (it == hash_table.end()) ? nullptr : it->second;
  }

  void Set1(Node *node, size_t x, size_t h) {
    node->min_value = node->max_value = x;
    node->mask.Set1(Index(x, h));
  }

  void AddNode(size_t x, size_t h) {
    auto node = nodes_manager.New();
    hash_table[HKey(x, h)] = node;
    Set1(node, x, h);
  }

  void DeleteNode(size_t x, size_t h) {
    auto key = HKey(x, h);
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
    maxh = 0;
    for (;; ++maxh) {
      size_t bits = (maxh + 1) * bits_per_level;
      if ((bits >= 64) || ((1ull << bits) >= usize)) break;
    }
    path.resize(maxh + 1);
  }

  size_t USize() const { return usize; }

  bool HasKey(size_t x) const {
    if (maxh == 0) return root.mask.HasKey(x);
    unsigned h0 = 0, h1 = maxh;
    for (; h0 < h1;) {
      const unsigned h = (h0 + h1) / 2;
      if (FindNode(x, h))
        h1 = h;
      else
        h0 = h + 1;
    }
    const auto node = (h0 == maxh) ? &root : FindNode(x, h0);
    return (h0 == 0) ? node->mask.HasKey(Index(x, 0)) : (x == node->min_value);
  }

  void Insert(size_t x) {
    if (root.IsEmpty()) return Set1(&root, x, maxh);
    Node *node = &root;
    for (size_t h = maxh;;) {
      // Split node if needed
      if (!node->IsSplit() && (h > 0)) {
        if (node->min_value == x) return;  // Adding same value
        AddNode(node->min_value, h - 1);
      }
      node->min_value = std::min(node->min_value, x);
      node->max_value = std::max(node->max_value, x);
      const size_t idx = Index(x, h);
      if (node->mask.HasKey(idx)) {
        if (h == 0) return;
        node = FindNode(x, --h);
      } else {
        node->mask.Insert(idx);
        if (h == 0) return;
        return AddNode(x, h - 1);
      }
    }
  }

  void Delete(size_t x) {
    Node *prev_node = nullptr, *node = &root;
    size_t prev_idx = 0, h = maxh;
    for (;; node = FindNode(x, --h)) {
      path[h] = node;
      if (!node->IsSplit()) {
        if (node->min_value != x) return;  // Removing missing element
        DeleteNode(x, h);
        if (prev_node) {
          assert(prev_node->mask.HasKey(prev_idx));
          prev_node->mask.Delete(prev_idx);
        }
      } else {
        prev_idx = Index(x, h);
        if (!node->mask.HasKey(prev_idx)) return;  // Removing missing element
        if (h == 0) {
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
    for (++h; h <= maxh; ++h) {
      node = path[h];
      if (node->min_value == x) {
        const auto x2 =
            HBits(x, h) + (node->mask.MinI() << (h * bits_per_level));
        node->min_value = FindNode(x2, h - 1)->min_value;
      } else if (node->max_value == x) {
        const auto x2 =
            HBits(x, h) + (node->mask.MaxI() << (h * bits_per_level));
        node->max_value = FindNode(x2, h - 1)->max_value;
      } else {
        break;
      }
      if (node->min_value == node->max_value)
        DeleteNode(node->min_value, h - 1);
    }
  }

  size_t Min() const { return root.min_value; }

  size_t Max() const { return root.max_value; }

  size_t Successor(size_t x) const {
    if (maxh == 0) return root.mask.Successor(x);
    if (root.IsEmpty() || (x >= root.max_value)) return kEmpty;
    unsigned h0 = 0, h1 = maxh;
    for (; h0 < h1;) {
      const unsigned h = (h0 + h1) / 2;
      const auto node = FindNode(x, h);
      if (node && (x < node->max_value))
        h1 = h;
      else
        h0 = h + 1;
    }
    const auto node = (h0 == maxh) ? &root : FindNode(x, h0);
    const size_t idx = Index(x, h0);
    if (h0 == 0) return x - idx + node->mask.Successor(idx);
    if (!node->IsSplit()) return node->max_value;
    const auto idx2 = node->mask.Successor(idx);
    return FindNode(x + ((idx2 - idx) << (h0 * bits_per_level)), h0 - 1)
        ->min_value;
  }

  size_t Predecessor(size_t x) const {
    if (maxh == 0) return root.mask.Predecessor(x);
    if (root.IsEmpty() || (x <= root.min_value)) return kEmpty;
    unsigned h0 = 0, h1 = maxh;
    for (; h0 < h1;) {
      const unsigned h = (h0 + h1) / 2;
      const auto node = FindNode(x, h);
      if (node && (x > node->min_value))
        h1 = h;
      else
        h0 = h + 1;
    }
    const auto node = (h0 == maxh) ? &root : FindNode(x, h0);
    const size_t idx = Index(x, h0);
    if (h0 == 0) return x - idx + node->mask.Predecessor(idx);
    if (!node->IsSplit()) return node->min_value;
    const auto idx2 = node->mask.Predecessor(idx);
    return FindNode(x - ((idx - idx2) << (h0 * bits_per_level)), h0 - 1)
        ->max_value;
  }
};
}  // namespace fus
}  // namespace ds
