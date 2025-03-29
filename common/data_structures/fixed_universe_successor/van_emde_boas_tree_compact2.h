#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <unordered_map>

namespace ds {
namespace fus {
// Van Emde Boas tree with one hash table for nodes.
// Use TFLS class on low level.
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log log U)
// HasKey      -- O(log log U)
// Delete      -- O(log log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
template <class TFLS>
class VanEmdeBoasTreeCompact2 {
 protected:
  static constexpr unsigned bits_per_level = TFLS::nbits;

  class Node;

  class NodeI {
   public:
    size_t min_value, max_value;
    Node* aux_tree;

   public:
    constexpr void Clear() {
      min_value = max_value = kEmpty;
      aux_tree = nullptr;
    }
  };

  class Node : public memory::Node {
   public:
    union {
      TFLS leaf;
      NodeI node;
    };

   public:
    constexpr Node() {}

    constexpr void Clear(unsigned h) {
      if (h == 1)
        leaf.Clear();
      else
        node.Clear();
    }

    constexpr bool IsEmpty(unsigned h) const {
      return (h == 1) ? leaf.IsEmpty() : (node.min_value == kEmpty);
    }

    constexpr size_t Min(unsigned h) const {
      return (h == 1) ? leaf.Min() : node.min_value;
    }

    constexpr size_t Max(unsigned h) const {
      return (h == 1) ? leaf.Max() : node.max_value;
    }

    constexpr void Set1(size_t x, unsigned h) {
      if (h == 1) {
        leaf.Set1(x);
      } else {
        node.min_value = node.max_value = x;
        node.aux_tree = nullptr;
      }
    }
  };

 protected:
  memory::NodesManager<Node> nodes_manager;
  Node root;
  std::unordered_map<uint64_t, Node*> nodes;
  unsigned maxh;
  size_t usize;

 protected:
  static size_t ClearLowBits(size_t x, unsigned l) {
    return (l < 64) ? ((x >> l) << l) : 0;
  }

  static size_t Replace(size_t x, size_t new_value, unsigned h0, unsigned h) {
    // Low h0 * bits_per_level bits will be cleared.
    auto l0 = h0 * bits_per_level, l = h * bits_per_level;
    return (((x >> (l0 + l)) << l) + new_value) << l0;
  }

  static uint64_t HKey(size_t x0, unsigned h0, unsigned h) {
    // assert(h0 + h < 16);
    return ClearLowBits(x0, bits_per_level * (h0 + h)) + (h0 << 4) + h0 + h;
  }

  Node* FindNode(uint64_t hkey) {
    auto it = nodes.find(hkey);
    return (it == nodes.end()) ? nullptr : it->second;
  }

  const Node* FindNode(uint64_t hkey) const {
    auto it = nodes.find(hkey);
    return (it == nodes.end()) ? nullptr : it->second;
  }

  Node* FindNode(size_t x0, unsigned h0, unsigned h) {
    return FindNode(HKey(x0, h0, h));
  }

  const Node* FindNode(size_t x0, unsigned h0, unsigned h) const {
    return FindNode(HKey(x0, h0, h));
  }

  Node* Make1(size_t x, unsigned h) {
    auto node = nodes_manager.create();
    node->Set1(x, h);
    return node;
  }

  void Make1(uint64_t hkey, size_t x, unsigned h) {
    assert(nodes.find(hkey) == nodes.end());
    nodes[hkey] = Make1(x, h);
  }

  void DeleteNode(uint64_t hkey, Node* node) {
    nodes_manager.release(node);
    nodes.erase(hkey);
  }

 public:
  VanEmdeBoasTreeCompact2() { Init(1); }

  void Clear() {
    nodes_manager.clear();
    nodes.clear();
    root.Clear(maxh);
  }

  void Init(size_t u) {
    usize = u;
    maxh = u ? numeric::ULog2(uint64_t(u - 1)) / bits_per_level + 1 : 1;
    Clear();
  }

  bool IsEmpty() const { return root.IsEmpty(maxh); }

  size_t Min() const { return root.Min(maxh); }

  size_t Max() const { return root.Max(maxh); }

  bool HasKey(size_t x) const {
    const size_t x0 = x;
    unsigned h = maxh;
    for (const Node* node = &root; node; node = FindNode(x0, 0, h)) {
      if (h == 1) return node->leaf.HasKey(x);
      if (x < node->node.min_value) return false;
      if (x == node->node.min_value) return true;
      h /= 2;
      x &= (1ull << (bits_per_level * h)) - 1;
    }
    return false;
  }

  void Insert(size_t x) {
    if (IsEmpty()) return root.Set1(x, maxh);
    size_t x0 = x;
    unsigned h0 = 0, h = maxh;
    for (Node* node = &root; node;) {
      if (h == 1) return node->leaf.Insert(x);
      if (x < node->node.min_value) {
        x0 += (node->node.min_value - x) << (bits_per_level * h0);
        std::swap(x, node->node.min_value);
      } else {
        node->node.max_value = std::max(node->node.max_value, x);
      }
      const auto hl = h / 2;
      const auto child_hkey = HKey(x0, h0, hl);
      const auto child = FindNode(child_hkey);
      if (!child) {
        Make1(child_hkey, x & ((1ull << (bits_per_level * hl)) - 1), hl);
        if (!node->node.aux_tree) {
          node->node.aux_tree = Make1(x >> (bits_per_level * hl), h - hl);
          return;
        }
        x >>= (bits_per_level * hl);
        h0 += hl;
        h -= h / 2;
        node = node->node.aux_tree;
      } else {
        x &= (1ull << (bits_per_level * hl)) - 1;
        h = hl;
        node = child;
      }
    }
  }

 protected:
  void DeleteI(Node* node, size_t x0, size_t x, unsigned h0, unsigned h) {
    if (h == 1) return node->leaf.Delete(x);
    const auto hl = h / 2;
    if (x == node->node.min_value) {
      if (node->node.min_value == node->node.max_value) {
        node->node.min_value = node->node.max_value = kEmpty;
        return;
      }
      assert(node->node.aux_tree);
      x0 = Replace(x0, node->node.aux_tree->Min(h - hl), h0 + hl, h - hl);
      const auto child = FindNode(x0, h0, hl);
      assert(child);
      x0 += (child->Min(hl) << (bits_per_level * h0));
      x = (node->node.aux_tree->Min(h - hl) << ((bits_per_level * hl))) +
          child->Min(hl);
      node->node.min_value = x;
    }
    const auto child_key = HKey(x0, h0, hl);
    const auto child = FindNode(child_key);
    if (!child) return;  // No element to remove
    DeleteI(child, x0, x & ((1ull << (bits_per_level * hl)) - 1), h0, hl);
    if (child->IsEmpty(hl)) {
      DeleteI(node->node.aux_tree, x0, x >> (bits_per_level * hl), h0 + hl,
              h - hl);
      if (node->node.aux_tree->IsEmpty(h - hl)) {
        nodes_manager.release(node->node.aux_tree);
        node->node.aux_tree = nullptr;
      }
      DeleteNode(child_key, child);
    }
    if (x == node->node.max_value) {
      if (!node->node.aux_tree) {
        node->node.max_value = node->node.min_value;
      } else {
        assert(!node->node.aux_tree->IsEmpty(h - hl));
        auto y = node->node.aux_tree->Max(h - hl);
        auto child = FindNode(Replace(x0, y, h0 + hl, h - hl), h0, hl);
        assert(child);
        node->node.max_value = (y << (bits_per_level * hl)) + child->Max(hl);
      }
    }
  }

  size_t SuccessorI(const Node* node, size_t x0, size_t x, unsigned h0,
                    unsigned h) const {
    if (h == 1) return node->leaf.Successor(x);
    if (x < node->node.min_value) return node->node.min_value;
    if (x >= node->node.max_value) return kEmpty;
    const unsigned hl = h / 2;
    size_t x1 = (x >> (bits_per_level * hl)),
           x2 = x & ((1ull << (bits_per_level * hl)) - 1);
    auto child = FindNode(x0, h0, hl);
    if (child && (x2 < child->Max(hl)))
      return x - x2 + SuccessorI(child, x0, x2, h0, hl);
    assert(node->node.aux_tree);
    x1 = SuccessorI(node->node.aux_tree, x0, x1, h0 + hl, h - hl);
    child = FindNode(Replace(x0, x1, h0 + hl, h - hl), h0, hl);
    assert(child);
    return (x1 << (bits_per_level * hl)) + child->Min(hl);
  }

  size_t PredecessorI(const Node* node, size_t x0, size_t x, unsigned h0,
                      unsigned h) const {
    if (h == 1) return node->leaf.Predecessor(x);
    if (x > node->node.max_value) return node->node.max_value;
    if (x <= node->node.min_value) return kEmpty;
    const unsigned hl = h / 2;
    size_t x1 = (x >> (bits_per_level * hl)),
           x2 = x & ((1ull << (bits_per_level * hl)) - 1);
    auto child = FindNode(x0, h0, hl);
    if (child && (x2 > child->Min(hl)))
      return x - x2 + PredecessorI(child, x0, x2, h0, hl);
    assert(node->node.aux_tree);
    x1 = PredecessorI(node->node.aux_tree, x0, x1, h0 + hl, h - hl);
    if (x1 == kEmpty) return node->node.min_value;
    child = FindNode(Replace(x0, x1, h0 + hl, h - hl), h0, hl);
    assert(child);
    return (x1 << (bits_per_level * hl)) + child->Max(hl);
  }

 public:
  void Delete(size_t x) { DeleteI(&root, x, x, 0, maxh); }

  size_t Successor(size_t x) const { return SuccessorI(&root, x, x, 0, maxh); }

  size_t Predecessor(size_t x) const {
    return PredecessorI(&root, x, x, 0, maxh);
  }
};
}  // namespace fus
}  // namespace ds
