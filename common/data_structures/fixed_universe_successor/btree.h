#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/blocks_manager.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <cstring>
#include <vector>

namespace ds {
namespace fus {
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
template <class TFLS>
class BTree {
 protected:
  static constexpr unsigned bits_per_level = TFLS::nbits;
  static constexpr size_t level_mask = (size_t(1) << bits_per_level) - 1;
  static constexpr size_t level_size = (size_t(1) << bits_per_level);

  class Node : public memory::Node {
   public:
    size_t min_value = Empty;
    size_t max_value = Empty;
    TFLS mask;
    Node** children = nullptr;

   public:
    constexpr bool IsEmpty() const { return min_value == Empty; }
    constexpr bool IsSplit() const { return children != nullptr; }

    constexpr void Clear() {
      min_value = max_value = Empty;
      mask.Clear();
      children = nullptr;
    }
  };

  using TNodeManager1 = memory::NodesManager<Node>;
  using TNodeManager2 = memory::BlocksManager<Node*>;

 protected:
  TNodeManager1 manager1;
  TNodeManager2 manager2;
  Node root;
  unsigned maxh;
  size_t usize;
  std::vector<Node*> path;

 protected:
  constexpr size_t Index(size_t x, size_t h) const {
    return (x >> (h * bits_per_level)) & level_mask;
  }

  constexpr void Set1(Node* node, size_t x, unsigned h) {
    node->min_value = node->max_value = x;
    node->mask.Set1(Index(x, h));
    node->children = nullptr;
  }

  constexpr Node* NewNode(size_t x, unsigned h) {
    auto node = manager1.New();
    Set1(node, x, h);
    return node;
  }

 public:
  constexpr BTree() { manager2.InitBlockSize(level_size); }

  constexpr void Clear() {
    manager1.ResetNodes();
    manager2.Clear();
    root.Clear();
  }

  constexpr void Init(size_t u) {
    Clear();
    usize = u;
    maxh =
        (u > level_size) ? numeric::ULog2(uint64_t(u - 1)) / bits_per_level : 0;
    path.resize(maxh + 1);
  }

  constexpr bool HasKey(size_t x) const {
    auto node = &root;
    for (unsigned h = maxh; node && (h > 0); --h) {
      if (!node->IsSplit()) return x == node->min_value;
      node = node->children[Index(x, h)];
    }
    return node ? node->mask.HasKey(x & level_mask) : false;
  }

  constexpr void Insert(size_t x) {
    if (root.IsEmpty()) return Set1(&root, x, maxh);
    auto node = &root;
    for (unsigned h = maxh; h; --h) {
      // Split node if needed
      if (!node->IsSplit()) {
        if (node->min_value == x) return;  // Adding same value
        node->children = manager2.NewBlock();
        memset(node->children, 0, level_size * sizeof(Node*));
        node->children[Index(node->min_value, h)] =
            NewNode(node->min_value, h - 1);
      }
      node->min_value = std::min(node->min_value, x);
      node->max_value = std::max(node->max_value, x);
      const size_t idx = Index(x, h);
      auto child = node->children[idx];
      if (child) {
        node = child;
      } else {
        node->mask.Insert(idx);
        node->children[idx] = NewNode(x, h - 1);
        return;
      }
    }
    node->min_value = std::min(node->min_value, x);
    node->max_value = std::max(node->max_value, x);
    node->mask.Insert(x & level_mask);
  }

  constexpr void Delete(size_t x) {
    Node *prev_node = nullptr, *node = &root;
    size_t idx = 0;
    unsigned h = maxh;
    for (;; --h) {
      path[h] = node;
      if (node->min_value == node->max_value) {
        if (node->min_value != x) return;  // Removing missing element
        if (prev_node) {
          manager1.Release(node);
          prev_node->mask.Delete(idx);
          prev_node->children[idx] = nullptr;
        }
        break;
      }
      if (h == 0) {
        idx = x & level_mask;
        node->mask.Delete(idx);
        if (node->min_value == x) {
          node->min_value = x - idx + node->mask.MinI();
        } else if (node->max_value == x) {
          node->max_value = x - idx + node->mask.MaxI();
        }
        break;
      }
      idx = Index(x, h);
      auto child = node->children[idx];
      if (!child) return;  // Removing missing element
      prev_node = node;
      node = child;
    }
    for (++h; h <= maxh; ++h) {
      node = path[h];
      if (node->min_value == x) {
        node->min_value = node->children[node->mask.MinI()]->min_value;
      } else if (node->max_value == x) {
        node->max_value = node->children[node->mask.MaxI()]->max_value;
      } else {
        break;
      }
      if (node->min_value == node->max_value) {
        auto child = node->children[Index(node->min_value, h)];
        manager1.Release(child);
        manager2.ReleaseBlock(node->children);
        node->children = nullptr;
      }
    }
  }

  constexpr size_t USize() const { return usize; }

  constexpr size_t Min() const { return root.min_value; }

  constexpr size_t Max() const { return root.max_value; }

  constexpr size_t Successor(size_t x) const {
    if (root.IsEmpty() || (x >= root.max_value)) return Empty;
    const Node* node = &root;
    for (size_t h = maxh; h; --h) {
      if (!node->IsSplit()) return node->max_value;
      const size_t idx = Index(x, h);
      auto child = node->children[idx];
      if (!child || (x >= child->max_value))
        return node->children[node->mask.Successor(idx)]->min_value;
      node = child;
    }
    const auto idx = x & level_mask;
    return x - idx + node->mask.Successor(idx);
  }

  constexpr size_t Predecessor(size_t x) const {
    if (root.IsEmpty() || (x <= root.min_value)) return Empty;
    const Node* node = &root;
    for (size_t h = maxh; h; --h) {
      if (!node->IsSplit()) return node->min_value;
      const size_t idx = Index(x, h);
      auto child = node->children[idx];
      if (!child || (x <= child->min_value))
        return node->children[node->mask.Predecessor(idx)]->max_value;
      node = child;
    }
    const auto idx = x & level_mask;
    return x - idx + node->mask.Predecessor(idx);
  }
};
}  // namespace fus
}  // namespace ds
