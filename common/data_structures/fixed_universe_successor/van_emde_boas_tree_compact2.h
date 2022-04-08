#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/node.h"
#include "common/nodes_manager.h"
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
  static const unsigned bits_per_level = TFLS::nbits;

  // TODO:
  // 1. Drop h
  // 2. Split Node based on h to Node1 and NodeI
  // 3. aux_tree link in the node instead of hash_table?
  class Node : public BaseNode {
   public:
    unsigned h;  // Not needed but simplify code
    TFLS leaf;
    size_t min_value, max_value;

   public:
    bool Flat() const { return h == 1; }

    bool IsEmpty() const {
      return Flat() ? leaf.IsEmpty() : (min_value == Empty);
    }

    size_t Min() const { return Flat() ? leaf.Min() : min_value; }
    size_t Max() const { return Flat() ? leaf.Max() : max_value; }

    void Clear() {
      if (Flat())
        leaf.Clear();
      else
        min_value = max_value = Empty;
    }

    void Set1(size_t x) {
      if (Flat())
        leaf.Set1I(x);
      else
        min_value = max_value = x;
    }
  };

 protected:
  NodesManager<Node> nodes_manager;
  Node* root;
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

  void Make1(uint64_t hkey, size_t x, unsigned h) {
    assert(nodes.find(hkey) == nodes.end());
    auto node = nodes_manager.New();
    node->h = h;
    node->Set1(x);
    nodes[hkey] = node;
  }

  void DeleteNode(uint64_t hkey, Node* node) {
    nodes_manager.Release(node);
    nodes.erase(hkey);
  }

 public:
  VanEmdeBoasTreeCompact2() { Clear(); }

  void Clear() {
    nodes_manager.ResetNodes();
    nodes.clear();
    root = nullptr;
  }

  void Init(size_t u) {
    Clear();
    usize = u;
    maxh = u ? numeric::ULog2(u - 1) / bits_per_level + 1 : 1;
    root = nodes_manager.New();
    root->h = maxh;
    root->Clear();
  }

  bool IsEmpty() const { return root->IsEmpty(); }
  size_t Min() const { return root->Min(); }
  size_t Max() const { return root->Max(); }

  bool HasKey(size_t x) const {
    size_t x0 = x;
    for (const Node* node = root; node;) {
      if (node->Flat()) return node->leaf.HasKey(x);
      if (x < node->min_value) return false;
      if (x == node->min_value) return true;
      x &= (1ull << bits_per_level * (node->h / 2)) - 1;
      node = FindNode(x0, 0, node->h / 2);
    }
    return false;
  }

  void Insert(size_t x) {
    if (IsEmpty()) return root->Set1(x);
    size_t x0 = x;
    unsigned h0 = 0;
    for (Node* node = root; node;) {
      if (node->Flat()) return node->leaf.Insert(x);
      if (x < node->min_value) {
        x0 += (node->min_value - x) << (bits_per_level * h0);
        std::swap(x, node->min_value);
      } else {
        node->max_value = std::max(node->max_value, x);
      }
      auto h = node->h, hl = h / 2;
      auto child_hkey = HKey(x0, h0, hl);
      auto child = FindNode(child_hkey);
      if (!child) {
        Make1(child_hkey, x & ((1ull << (bits_per_level * hl)) - 1), hl);
        auto parent_hkey = HKey(x0, h0 + hl, h - hl);
        auto parent = FindNode(parent_hkey);
        if (!parent)
          return Make1(parent_hkey, x >> (bits_per_level * hl), h - hl);
        x >>= (bits_per_level * hl);
        h0 += hl;
        node = parent;
      } else {
        x &= (1ull << (bits_per_level * hl)) - 1;
        node = child;
      }
    }
    assert(false);
  }

 protected:
  void DeleteI(Node* node, size_t x0, size_t x, unsigned h0) {
    if (node->Flat()) return node->leaf.Delete(x);
    auto h = node->h, hl = h / 2;
    auto parent_key = HKey(x0, h0 + hl, h - hl);
    if (x == node->min_value) {
      if (node->min_value == node->max_value) {
        node->min_value = node->max_value = Empty;
        return;
      }
      auto parent = FindNode(parent_key);
      assert(parent);
      x0 = Replace(x0, parent->Min(), h0 + hl, h - hl);
      auto child = FindNode(x0, h0, hl);
      assert(child);
      x0 += (child->Min() << (bits_per_level * h0));
      x = (parent->Min() << ((bits_per_level * hl))) + child->Min();
      node->min_value = x;
    }
    auto child_key = HKey(x0, h0, hl);
    auto child = FindNode(child_key);
    if (!child) {
      // No element to remove
      assert(false);
      return;
    }
    DeleteI(child, x0, x & ((1ull << (bits_per_level * hl)) - 1), h0);
    if (child->IsEmpty()) {
      auto parent = FindNode(parent_key);
      DeleteI(parent, x0, x >> (bits_per_level * hl), h0 + hl);
      if (parent->IsEmpty()) {
        DeleteNode(parent_key, parent);
      }
      DeleteNode(child_key, child);
    }
    if (x == node->max_value) {
      auto parent = FindNode(parent_key);
      if (!parent) {
        node->max_value = node->min_value;
      } else {
        assert(!parent->IsEmpty());
        auto y = parent->Max();
        auto child = FindNode(Replace(x0, y, h0 + hl, h - hl), h0, hl);
        assert(child);
        node->max_value = (y << (bits_per_level * hl)) + child->Max();
      }
    }
  }

  size_t SuccessorI(const Node* node, size_t x0, size_t x, unsigned h0) const {
    if (node->Flat()) return node->leaf.Successor(x);
    if (x < node->min_value) return node->min_value;
    if (x >= node->max_value) return Empty;
    unsigned h = node->h, hl = h / 2;
    size_t x1 = (x >> (bits_per_level * hl)),
           x2 = x & ((1ull << (bits_per_level * hl)) - 1);
    auto child = FindNode(x0, h0, hl);
    if (child && (x2 < child->Max()))
      return x - x2 + SuccessorI(child, x0, x2, h0);
    auto parent = FindNode(x0, h0 + hl, h - hl);
    assert(parent);
    x1 = SuccessorI(parent, x0, x1, h0 + hl);
    child = FindNode(Replace(x0, x1, h0 + hl, h - hl), h0, hl);
    assert(child);
    return (x1 << (bits_per_level * hl)) + child->Min();
  }

  size_t PredecessorI(const Node* node, size_t x0, size_t x,
                      unsigned h0) const {
    if (node->Flat()) return node->leaf.Predecessor(x);
    if (x > node->max_value) return node->max_value;
    if (x <= node->min_value) return Empty;
    unsigned h = node->h, hl = h / 2;
    size_t x1 = (x >> (bits_per_level * hl)),
           x2 = x & ((1ull << (bits_per_level * hl)) - 1);
    auto child = FindNode(x0, h0, hl);
    if (child && (x2 > child->Min()))
      return x - x2 + PredecessorI(child, x0, x2, h0);
    auto parent = FindNode(x0, h0 + hl, h - hl);
    assert(parent);
    x1 = PredecessorI(parent, x0, x1, h0 + hl);
    if (x1 == Empty) return node->min_value;
    child = FindNode(Replace(x0, x1, h0 + hl, h - hl), h0, hl);
    assert(child);
    return (x1 << (bits_per_level * hl)) + child->Max();
  }

 public:
  void Delete(size_t x) { DeleteI(root, x, x, 0); }
  size_t Successor(size_t x) const { return SuccessorI(root, x, x, 0); }
  size_t Predecessor(size_t x) const { return PredecessorI(root, x, x, 0); }
};
}  // namespace fus
}  // namespace ds
