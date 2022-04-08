#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
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
class BTrie {
 protected:
  static const unsigned bits_per_level = TFLS::nbits;
  static const size_t level_mask = (size_t(1) << bits_per_level) - 1;
  static const size_t level_size = (size_t(1) << bits_per_level);

  class Node : public BaseNode {
   public:
    TFLS mask;
    Node* p;
    std::vector<Node*> children;
  };

  using TNodeManager = NodesManager<Node>;

 protected:
  TNodeManager manager1, manager2;
  Node* root;
  unsigned maxh;
  size_t usize, size;

 protected:
  void ReleaseNode(Node* node) {
    (node->children.empty() ? manager2 : manager1).Release(node);
  }

  void ReleaseR(Node* node) {
    if (node->children.empty()) return;
    for (; !node->mask.IsEmpty();) {
      auto v = node->mask.Min();
      ReleaseR(node->children[v]);
      ReleaseNode(node->children[v]);
      node->children[v] = nullptr;
      node->mask.Delete(v);
    }
  }

 public:
  BTrie() {
    root = manager1.New();
    root->p = nullptr;
  }

  void Clear() {
    size = 0;
    ReleaseR(root);
  }

  void Init(size_t u) {
    Clear();
    usize = u;
    maxh = (u > level_size) ? numeric::ULog2(u - 1) / bits_per_level + 1 : 1;
    root->children.resize(maxh > 1 ? level_size : 0, nullptr);
  }

  bool HasKey(size_t x) const {
    auto node = root;
    for (unsigned h = 1; node && (h < maxh); ++h) {
      node = node->children[(x >> (bits_per_level * (maxh - h))) & level_mask];
    }
    return node ? node->mask.HasKey(x & level_mask) : false;
  }

  void Insert(size_t x) {
    auto node = root;
    for (unsigned h = 1; h < maxh; ++h) {
      auto v = (x >> (bits_per_level * (maxh - h))) & level_mask;
      if (!node->children[v]) {
        Node* nodec = nullptr;
        if ((h + 1) < maxh) {
          nodec = manager1.New();
          nodec->children.resize(level_size, nullptr);
        } else {
          nodec = manager2.New();
        }
        node->children[v] = nodec;
        nodec->p = node;
        node->mask.Insert(v);
      }
      node = node->children[v];
    }
    if (!node->mask.HasKey(x & level_mask)) {
      ++size;
      node->mask.Insert(x & level_mask);
    }
  }

  void Delete(size_t x) {
    auto node = root;
    for (unsigned h = 1; node && (h < maxh); ++h) {
      node = node->children[(x >> (bits_per_level * (maxh - h))) & level_mask];
    }
    if (!node || !node->mask.HasKey(x & level_mask))
      return;  // No element to remove
    --size;
    node->mask.Delete(x & level_mask);
    for (; node->mask.IsEmpty() && (node != root);) {
      node = node->p;
      x >>= bits_per_level;
      auto v = x & level_mask;
      node->mask.Delete(v);
      ReleaseNode(node->children[v]);
      node->children[v] = nullptr;
    }
  }

  size_t Size() const { return size; }
  size_t USize() const { return usize; }

 protected:
  size_t MinI(Node* node, size_t x) const {
    if (node->mask.IsEmpty()) return Empty;
    for (; !node->children.empty();) {
      auto v = node->mask.Min();
      x = (x << bits_per_level) + v;
      node = node->children[v];
    }
    return (x << bits_per_level) + node->mask.Min();
  }

  size_t MaxI(Node* node, size_t x) const {
    if (node->mask.IsEmpty()) return Empty;
    for (; !node->children.empty();) {
      auto v = node->mask.Max();
      x = (x << bits_per_level) + v;
      node = node->children[v];
    }
    return (x << bits_per_level) + node->mask.Max();
  }

 public:
  size_t Min() const { return MinI(root, 0); }
  size_t Max() const { return MaxI(root, 0); }

  size_t Successor(size_t x) const {
    auto node = root;
    unsigned h = 1;
    for (; h < maxh; ++h) {
      auto v = (x >> (bits_per_level * (maxh - h))) & level_mask;
      if (!node->children[v]) break;
      node = node->children[v];
    }
    for (; node; --h) {
      auto v = (x >> (bits_per_level * (maxh - h))) & level_mask;
      auto v1 = node->mask.Successor(v);
      if (v1 != Empty) {
        auto xa = (x >> (bits_per_level * (maxh - h))) - v + v1;
        return node->children.empty() ? xa : MinI(node->children[v1], xa);
      }
      node = node->p;
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    auto node = root;
    unsigned h = 1;
    for (; h < maxh; ++h) {
      auto v = (x >> (bits_per_level * (maxh - h))) & level_mask;
      if (!node->children[v]) break;
      node = node->children[v];
    }
    for (; node; --h) {
      auto v = (x >> (bits_per_level * (maxh - h))) & level_mask;
      auto v1 = node->mask.Predecessor(v);
      if (v1 != Empty) {
        auto xa = (x >> (bits_per_level * (maxh - h))) - v + v1;
        return node->children.empty() ? xa : MaxI(node->children[v1], xa);
      }
      node = node->p;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
