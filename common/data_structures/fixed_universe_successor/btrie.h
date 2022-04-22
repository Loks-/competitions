#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
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

  class Node : public memory::Node {
   public:
    TFLS mask;
    Node* p;
    std::vector<Node*> children;
  };

  using TNodeManager = memory::NodesManager<Node>;

 protected:
  TNodeManager manager1, manager2;
  Node root;
  unsigned maxh;
  size_t usize, size;

 protected:
  size_t Index(size_t x, size_t h) const {
    return (x >> (h * bits_per_level)) & level_mask;
  }

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
  BTrie() { root.p = nullptr; }

  void Clear() {
    size = 0;
    ReleaseR(&root);
  }

  void Init(size_t u) {
    Clear();
    usize = u;
    maxh = (u > level_size) ? numeric::ULog2(u - 1) / bits_per_level : 0;
    root.children.resize(maxh > 0 ? level_size : 0, nullptr);
  }

  bool HasKey(size_t x) const {
    auto node = &root;
    for (unsigned h = maxh; node && (h > 0); --h)
      node = node->children[Index(x, h)];
    return node ? node->mask.HasKey(x & level_mask) : false;
  }

  void Insert(size_t x) {
    auto node = &root;
    for (unsigned h = maxh; h; --h) {
      size_t idx = Index(x, h);
      if (!node->children[idx]) {
        Node* child = nullptr;
        if (h > 1) {
          child = manager1.New();
          child->children.resize(level_size, nullptr);
        } else {
          child = manager2.New();
        }
        node->children[idx] = child;
        child->p = node;
        node->mask.Insert(idx);
      }
      node = node->children[idx];
    }
    if (!node->mask.HasKey(x & level_mask)) {
      ++size;
      node->mask.Insert(x & level_mask);
    }
  }

  void Delete(size_t x) {
    auto node = &root;
    for (unsigned h = maxh; h; --h) node = node->children[Index(x, h)];
    if (!node || !node->mask.HasKey(x & level_mask))
      return;  // No element to remove
    --size;
    node->mask.Delete(x & level_mask);
    for (; node->mask.IsEmpty() && (node != &root);) {
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
  size_t MinI(const Node* node, size_t x) const {
    if (node->mask.IsEmpty()) return Empty;
    for (; !node->children.empty();) {
      auto v = node->mask.Min();
      x = (x << bits_per_level) + v;
      node = node->children[v];
    }
    return (x << bits_per_level) + node->mask.Min();
  }

  size_t MaxI(const Node* node, size_t x) const {
    if (node->mask.IsEmpty()) return Empty;
    for (; !node->children.empty();) {
      auto v = node->mask.Max();
      x = (x << bits_per_level) + v;
      node = node->children[v];
    }
    return (x << bits_per_level) + node->mask.Max();
  }

 public:
  size_t Min() const { return MinI(&root, 0); }
  size_t Max() const { return MaxI(&root, 0); }

  size_t Successor(size_t x) const {
    auto node = &root;
    unsigned h = maxh;
    for (; h; --h) {
      auto idx = Index(x, h);
      if (!node->children[idx]) break;
      node = node->children[idx];
    }
    for (; node; ++h) {
      auto idx = Index(x, h);
      auto idx1 = node->mask.Successor(idx);
      if (idx1 != Empty) {
        auto xa = (x >> (bits_per_level * h)) - idx + idx1;
        return node->children.empty() ? xa : MinI(node->children[idx1], xa);
      }
      node = node->p;
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    auto node = &root;
    unsigned h = maxh;
    for (; h; --h) {
      auto idx = Index(x, h);
      if (!node->children[idx]) break;
      node = node->children[idx];
    }
    for (; node; ++h) {
      auto idx = Index(x, h);
      auto idx1 = node->mask.Predecessor(idx);
      if (idx1 != Empty) {
        auto xa = (x >> (bits_per_level * h)) - idx + idx1;
        return node->children.empty() ? xa : MaxI(node->children[idx1], xa);
      }
      node = node->p;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
