#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/numeric/bits/first_bit.h"
#include "common/numeric/bits/ulog2.h"

#include <unordered_map>
#include <vector>

namespace ds {
namespace fus {
// X-fast tree, compact version for X-fast trie
// Memory      -- O(S log U)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(1)
// Delete      -- O(log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
class XFastTree {
 protected:
  class Node : public memory::Node {
   public:
    size_t value;
    Node *l, *r;
  };

  using TNodeManager = memory::NodesManager<Node>;

 protected:
  Node empty_node;
  TNodeManager node_manager;
  std::unordered_map<size_t, Node *> lm;
  std::vector<std::unordered_map<size_t, Node *>> vm;
  size_t size, usize;
  unsigned maxh;

 public:
  XFastTree() { Clear(); }

  XFastTree(size_t u) { Init(u); }

  void Clear() {
    size = 0;
    empty_node.value = kEmpty;
    empty_node.l = empty_node.r = &empty_node;
    node_manager.ResetNodes();
    lm.clear();
    for (auto &m : vm) m.clear();
  }

 public:
  void Init(size_t u) {
    Clear();
    usize = u;
    maxh = u ? numeric::ULog2(uint64_t(u - 1)) + 1 : 0;
    vm.resize(maxh + 1);
  }

  bool IsEmpty() const { return size == 0; }
  size_t Size() const { return size; }
  bool HasKey(size_t x) const { return lm.find(x) != lm.end(); }
  size_t Min() const { return empty_node.r->value; }
  size_t Max() const { return empty_node.l->value; }

 protected:
  unsigned FindH(size_t x) const {
    assert(!IsEmpty());
    unsigned h0 = 0u, h1 = maxh;
    for (; h1 > h0;) {
      const auto h = (h0 + h1 - 1) / 2;
      if (vm[h].find(x >> h) == vm[h].end()) {
        h0 = h + 1;
      } else {
        h1 = h;
      }
    }
    return h1;
  }

  void InsertFirstNode(size_t x) {
    auto node = node_manager.New();
    node->value = x;
    node->l = node->r = &empty_node;
    empty_node.l = empty_node.r = node;
    lm[x] = node;
    vm[maxh][x >> maxh] = node;
  }

 public:
  void Insert(size_t x) {
    if (HasKey(x)) return;
    if (++size == 1) return InsertFirstNode(x);
    auto h1 = FindH(x);
    auto snode = vm[h1][x >> h1];
    const auto sx = snode->value;
    auto noder = (x < sx) ? snode : snode->r;
    auto nodel = noder->l;
    auto node = node_manager.New();
    node->value = x;
    node->l = nodel;
    node->r = noder;
    nodel->r = node;
    noder->l = node;
    lm[x] = node;
    auto h0 = numeric::HighestBit(uint64_t(x ^ sx));
    auto xh = x >> h0;
    vm[h0][xh] = node;
    if (vm[h0].find(sx >> h0) == vm[h0].end()) vm[h0][sx >> h0] = snode;
    if (x < sx) {
      for (++h0; h0 < h1; ++h0, xh >>= 1)
        vm[h0][xh >> 1] = (xh & 1) ? node : snode;
      for (; h1 <= maxh; ++h1, xh >>= 1) {
        if ((xh & 1) == 1) {
          if (vm[h1][xh >> 1] == snode)
            vm[h1][xh >> 1] = node;
          else
            break;
        }
      }
    } else {
      for (++h0; h0 < h1; ++h0, xh >>= 1)
        vm[h0][xh >> 1] = (xh & 1) ? snode : node;
      for (; h1 <= maxh; ++h1, xh >>= 1) {
        if ((xh & 1) == 0) {
          if (vm[h1][xh >> 1] == snode)
            vm[h1][xh >> 1] = node;
          else
            break;
        }
      }
    }
  }

  void Delete(size_t x) {
    auto it = lm.find(x);
    if (it == lm.end()) return;       // No element to remove
    if (--size == 0) return Clear();  // Last element
    auto node = it->second;
    auto nodel = node->l, noder = node->r;
    nodel->r = noder;
    noder->l = nodel;
    lm.erase(it);
    node_manager.Release(node);
    auto h1 = FindH(x);
    auto xh = x >> h1;
    for (; vm[h1].find(xh ^ 1) == vm[h1].end(); ++h1, xh >>= 1)
      vm[h1].erase(xh);
    vm[h1].erase(xh);
    if (xh & 1) {
      xh >>= 1;
      vm[++h1][xh] = nodel;
      for (++h1; h1 <= maxh; ++h1, xh >>= 1) {
        if (~xh & 1) {
          if (vm[h1][xh >> 1] == node) {
            vm[h1][xh >> 1] = nodel;
          } else {
            break;
          }
        }
      }
    } else {
      xh >>= 1;
      vm[++h1][xh] = noder;
      for (++h1; h1 <= maxh; ++h1, xh >>= 1) {
        if (xh & 1) {
          if (vm[h1][xh >> 1] == node) {
            vm[h1][xh >> 1] = noder;
          } else {
            break;
          }
        }
      }
    }
  }

 protected:
  const Node *SuccessorI(size_t x) const {
    if (IsEmpty()) return &empty_node;
    const auto it = lm.find(x);
    if (it != lm.end()) return it->second->r;
    const auto h = FindH(x);
    const auto node = vm[h].find(x >> h)->second;
    return (node->value > x) ? node : node->r;
  }

  const Node *PredecessorI(size_t x) const {
    if (IsEmpty()) return &empty_node;
    const auto it = lm.find(x);
    if (it != lm.end()) return it->second->l;
    const auto h = FindH(x);
    const auto node = vm[h].find(x >> h)->second;
    return (node->value < x) ? node : node->l;
  }

 public:
  size_t Successor(size_t x) const { return SuccessorI(x)->value; }
  size_t Predecessor(size_t x) const { return PredecessorI(x)->value; }
};
}  // namespace fus
}  // namespace ds
