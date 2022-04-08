#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/fixed_length_set_6b.h"
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
class B64Trie {
 protected:
  class Node : public BaseNode {
   public:
    FLSetB6 mask;
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
  B64Trie() {
    root = manager1.New();
    root->p = nullptr;
    root->children.resize(64, nullptr);
  }

  void Clear() {
    size = 0;
    ReleaseR(root);
  }

  void Init(size_t u) {
    Clear();
    usize = u;
    maxh = (u > 64) ? numeric::ULog2(u - 1) / 6 + 1 : 1;
  }

  bool HasKey(size_t x) const {
    auto node = root;
    for (unsigned h = 1; node && (h < maxh); ++h) {
      node = node->children[(x >> (6 * (maxh - h))) & 63];
    }
    return node ? node->mask.HasKey(x & 63) : false;
  }

  void Insert(size_t x) {
    auto node = root;
    for (unsigned h = 1; h < maxh; ++h) {
      auto v = (x >> (6 * (maxh - h))) & 63;
      if (!node->children[v]) {
        Node* nodec = nullptr;
        if ((h + 1) < maxh) {
          nodec = manager1.New();
          nodec->children.resize(64, nullptr);
        } else {
          nodec = manager2.New();
        }
        node->children[v] = nodec;
        nodec->p = node;
        node->mask.Insert(v);
      }
      node = node->children[v];
    }
    if (!node->mask.HasKey(x & 63)) {
      ++size;
      node->mask.Insert(x & 63);
    }
  }

  void Delete(size_t x) {
    auto node = root;
    for (unsigned h = 1; node && (h < maxh); ++h) {
      node = node->children[(x >> (6 * (maxh - h))) & 63];
    }
    if (!node || !node->mask.HasKey(x & 63)) return;  // No element to remove
    --size;
    node->mask.Delete(x & 63);
    for (; node->mask.IsEmpty() && (node != root);) {
      node = node->p;
      x >>= 6;
      auto v = x & 63;
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
      x = (x << 6) + v;
      node = node->children[v];
    }
    return (x << 6) + node->mask.Min();
  }

  size_t MaxI(Node* node, size_t x) const {
    if (node->mask.IsEmpty()) return Empty;
    for (; !node->children.empty();) {
      auto v = node->mask.Max();
      x = (x << 6) + v;
      node = node->children[v];
    }
    return (x << 6) + node->mask.Max();
  }

 public:
  size_t Min() const { return MinI(root, 0); }
  size_t Max() const { return MaxI(root, 0); }

  size_t Successor(size_t x) const {
    auto node = root;
    unsigned h = 1;
    for (; h < maxh; ++h) {
      auto v = (x >> (6 * (maxh - h))) & 63;
      if (!node->children[v]) break;
      node = node->children[v];
    }
    for (; node; --h) {
      auto v = (x >> (6 * (maxh - h))) & 63;
      auto v1 = node->mask.Successor(v);
      if (v1 != Empty) {
        auto xa = (x >> (6 * (maxh - h))) - v + v1;
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
      auto v = (x >> (6 * (maxh - h))) & 63;
      if (!node->children[v]) break;
      node = node->children[v];
    }
    for (; node; --h) {
      auto v = (x >> (6 * (maxh - h))) & 63;
      auto v1 = node->mask.Predecessor(v);
      if (v1 != Empty) {
        auto xa = (x >> (6 * (maxh - h))) - v + v1;
        return node->children.empty() ? xa : MaxI(node->children[v1], xa);
      }
      node = node->p;
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
