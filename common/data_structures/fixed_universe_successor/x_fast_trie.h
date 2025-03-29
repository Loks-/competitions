#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/node.h"
#include "common/memory/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <unordered_map>
#include <vector>

namespace ds {
namespace fus {
// X-fast trie
// Memory      -- O(S log U)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(1)
// Delete      -- O(log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
class XFastTrie {
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
  std::vector<std::unordered_map<uint64_t, Node *>> vm;
  size_t size, usize;
  unsigned maxh;

 public:
  XFastTrie() { Clear(); }

  XFastTrie(size_t u) { Init(u); }

  void Clear() {
    size = 0;
    empty_node.value = kEmpty;
    empty_node.l = empty_node.r = &empty_node;
    node_manager.clear();
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
  bool HasKey(size_t x) const { return vm[0].find(x) != vm[0].end(); }
  size_t Min() const { return empty_node.r->value; }
  size_t Max() const { return empty_node.l->value; }

 protected:
  void InsertFirstNode(size_t x) {
    auto node = node_manager.create();
    node->value = x;
    node->l = node->r = &empty_node;
    empty_node.l = empty_node.r = node;
    vm[0][x] = node;
    auto last_node = node;
    for (unsigned h = 1; h <= maxh; ++h) {
      auto hnode = node_manager.create();
      if ((x >> (h - 1)) & 1) {
        hnode->l = node;
        hnode->r = last_node;
      } else {
        hnode->l = last_node;
        hnode->r = node;
      }
      vm[h][x >> h] = hnode;
      last_node = hnode;
    }
  }

 public:
  void Insert(size_t x) {
    if (HasKey(x)) return;
    if (++size == 1) return InsertFirstNode(x);
    auto noder = (Node *)SuccessorI(x);
    auto nodel = noder->l;
    auto node = node_manager.create();
    node->value = x;
    node->l = nodel;
    node->r = noder;
    nodel->r = node;
    noder->l = node;
    vm[0][x] = node;
    auto last_node = node;
    unsigned h = 1;
    uint64_t xh = x;
    for (;; ++h, xh >>= 1) {
      auto it = vm[h].find(xh >> 1);
      if (it == vm[h].end()) {
        auto hnode = node_manager.create();
        if (xh & 1) {
          hnode->l = node;
          hnode->r = last_node;
        } else {
          hnode->l = last_node;
          hnode->r = node;
        }
        vm[h][xh >> 1] = hnode;
        last_node = hnode;
      } else {
        auto hnode = it->second;
        ((xh & 1) ? hnode->r : hnode->l) = last_node;
        break;
      }
    }
    if (xh & 1) {
      for (++h, xh >>= 1; h <= maxh; ++h, xh >>= 1) {
        if (~xh & 1) {
          auto hnode = vm[h][xh >> 1];
          if (hnode->r == nodel)
            hnode->r = node;
          else
            break;
        }
      }
    } else {
      for (++h, xh >>= 1; h <= maxh; ++h, xh >>= 1) {
        if (xh & 1) {
          auto hnode = vm[h][xh >> 1];
          if (hnode->l == noder)
            hnode->l = node;
          else
            break;
        }
      }
    }
  }

  void Delete(size_t x) {
    auto it = vm[0].find(x);
    if (it == vm[0].end()) return;    // No element to remove
    if (--size == 0) return Clear();  // Last element
    auto node = it->second;
    auto nodel = node->l, noder = node->r;
    nodel->r = noder;
    noder->l = nodel;
    vm[0].erase(x);
    node_manager.release(node);
    unsigned h = 1;
    uint64_t xh = x;
    for (;; ++h, xh >>= 1) {
      auto hnode = vm[h][xh >> 1];
      if (vm[h - 1].find(xh ^ 1) == vm[h - 1].end()) {
        vm[h].erase(xh >> 1);
        node_manager.release(hnode);
      } else {
        if (xh & 1) {
          hnode->r = nodel;
        } else {
          hnode->l = noder;
        }
        break;
      }
    }
    if (xh & 1) {
      for (++h, xh >>= 1; h <= maxh; ++h, xh >>= 1) {
        if (~xh & 1) {
          auto hnode = vm[h][xh >> 1];
          if (hnode->r == node)
            hnode->r = nodel;
          else
            break;
        }
      }
    } else {
      for (++h, xh >>= 1; h <= maxh; ++h, xh >>= 1) {
        if (xh & 1) {
          auto hnode = vm[h][xh >> 1];
          if (hnode->l == node)
            hnode->l = noder;
          else
            break;
        }
      }
    }
  }

 protected:
  const Node *SuccessorI(size_t x) const {
    if (IsEmpty()) return &empty_node;
    unsigned h0 = 0u, h1 = maxh;
    for (; h1 > h0;) {
      const auto h = (h0 + h1 + 1) / 2 - 1;
      if (vm[h].find(x >> h) == vm[h].end()) {
        h0 = h + 1;
      } else {
        h1 = h;
      }
    }
    auto hnode = vm[h1].find(x >> h1)->second;
    return h1 ? ((x >> (h1 - 1)) & 1) ? hnode->r->r : hnode->l : hnode->r;
  }

  const Node *PredecessorI(size_t x) const {
    if (IsEmpty()) return &empty_node;
    unsigned h0 = 0u, h1 = maxh;
    for (; h1 > h0;) {
      const auto h = (h0 + h1 + 1) / 2 - 1;
      if (vm[h].find(x >> h) == vm[h].end()) {
        h0 = h + 1;
      } else {
        h1 = h;
      }
    }
    auto hnode = vm[h1].find(x >> h1)->second;
    return h1 ? ((x >> (h1 - 1)) & 1) ? hnode->r : hnode->l->l : hnode->l;
  }

 public:
  size_t Successor(size_t x) const { return SuccessorI(x)->value; }
  size_t Predecessor(size_t x) const { return PredecessorI(x)->value; }
};
}  // namespace fus
}  // namespace ds
