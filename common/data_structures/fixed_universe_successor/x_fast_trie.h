#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/node.h"
#include "common/nodes_manager.h"
#include "common/numeric/bits/ulog2.h"

#include <unordered_map>

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
  class Node : public BaseNode {
   public:
    size_t value;
    Node *l, *r;
  };

  using TNodeManager = NodesManager<Node>;

 protected:
  Node *empty_node;
  TNodeManager node_manager;
  std::unordered_map<uint64_t, Node *> m;
  size_t usize;
  unsigned maxh;

 protected:
  static uint64_t HXToKey(unsigned h, size_t x) {
    return (x >> h) + (uint64_t(h) << 58);
  }

  static unsigned KeyToH(uint64_t key) { return unsigned(key >> 58); }

  Node *Find(size_t x) {
    if (IsEmpty()) return empty_node;
    unsigned h0 = 0u, h1 = maxh;
    for (; h1 - h0 > 1;) {
      auto h = (h0 + h1) / 2;
      auto key = HXToKey(h - 1, x);
      if (m.find(key) == m.end()) {
        h0 = h;
      } else {
        h1 = h;
      }
    }
    auto r = m[HXToKey(h0, x)];
    return r;
  }

  const Node *Find(size_t x) const {
    if (IsEmpty()) return empty_node;
    unsigned h0 = 0u, h1 = maxh;
    for (; h1 - h0 > 1;) {
      auto h = (h0 + h1) / 2;
      auto key = HXToKey(h - 1, x);
      if (m.find(key) == m.end()) {
        h0 = h;
      } else {
        h1 = h;
      }
    }
    return m.find(HXToKey(h0, x))->second;
  }

 public:
  XFastTrie() {}
  XFastTrie(size_t u) { Init(u); }

  void Clear() {
    node_manager.ResetNodes();
    m.clear();
    empty_node = node_manager.New();
    empty_node->value = Empty;
    empty_node->l = empty_node->r = empty_node;
  }

 public:
  void Init(size_t u) {
    static_assert(sizeof(size_t) == sizeof(uint64_t));
    Clear();
    usize = u;
    maxh = u ? numeric::ULog2(u - 1) + 2 : 1;
    assert(maxh < 58);
  }

  bool IsEmpty() const { return m.empty(); }

  bool HasKey(size_t x) const { return m.find(HXToKey(0, x)) != m.end(); }
  size_t Min() const { return empty_node->r->value; }
  size_t Max() const { return empty_node->l->value; }

 protected:
  void InsertFirstNode(size_t x) {
    auto node = node_manager.New();
    node->value = x;
    node->l = node->r = empty_node;
    empty_node->l = empty_node->r = node;
    m[x] = node;
    auto last_node = node;
    for (unsigned h = 1; h < maxh; ++h) {
      auto hnode = node_manager.New();
      hnode->value = HXToKey(h, x);
      if ((x >> (h - 1)) & 1) {
        hnode->l = node;
        hnode->r = last_node;
      } else {
        hnode->l = last_node;
        hnode->r = node;
      }
      m[hnode->value] = hnode;
      last_node = hnode;
    }
  }

 public:
  void Insert(size_t x) {
    if (HasKey(x)) return;
    if (IsEmpty()) return InsertFirstNode(x);
    auto noder = SuccessorI(x);
    auto nodel = noder->l;
    auto node = node_manager.New();
    node->value = x;
    node->l = nodel;
    node->r = noder;
    nodel->r = node;
    noder->l = node;
    m[x] = node;
    auto last_node = node;
    uint64_t h = 1, xh = x;
    for (;; ++h, xh >>= 1) {
      auto key = (h << 58) + (xh >> 1);
      auto it = m.find(key);
      if (it == m.end()) {
        auto hnode = node_manager.New();
        hnode->value = key;
        if (xh & 1) {
          hnode->l = node;
          hnode->r = last_node;
        } else {
          hnode->l = last_node;
          hnode->r = node;
        }
        m[hnode->value] = hnode;
        last_node = hnode;
      } else {
        auto hnode = it->second;
        ((xh & 1) ? hnode->r : hnode->l) = last_node;
        break;
      }
    }
    if (xh & 1) {
      for (++h, xh >>= 1; h < maxh; ++h, xh >>= 1) {
        if (~xh & 1) {
          auto hnode = m[(h << 58) + (xh >> 1)];
          if (hnode->r == nodel)
            hnode->r = node;
          else
            break;
        }
      }
    } else {
      for (++h, xh >>= 1; h < maxh; ++h, xh >>= 1) {
        if (xh & 1) {
          auto hnode = m[(h << 58) + (xh >> 1)];
          if (hnode->l == noder)
            hnode->l = node;
          else
            break;
        }
      }
    }
  }

  void Delete(size_t x) {
    auto it = m.find(x);
    if (it == m.end()) return;  // No element to remove
    auto node = it->second;
    auto nodel = node->l, noder = node->r;
    if (nodel == noder) return Clear();  // Last element
    nodel->r = noder;
    noder->l = nodel;
    m.erase(x);
    uint64_t h = 1, xh = x;
    for (;; ++h, xh >>= 1) {
      auto hkey = (h << 58) + (xh >> 1), skey = ((h - 1) << 58) + (xh ^ 1);
      auto hnode = m[hkey];
      if (m.find(skey) == m.end()) {
        m.erase(hkey);
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
      for (++h, xh >>= 1; h < maxh; ++h, xh >>= 1) {
        if (~xh & 1) {
          auto hnode = m[(h << 58) + (xh >> 1)];
          if (hnode->r == node)
            hnode->r = nodel;
          else
            break;
        }
      }
    } else {
      for (++h, xh >>= 1; h < maxh; ++h, xh >>= 1) {
        if (xh & 1) {
          auto hnode = m[(h << 58) + (xh >> 1)];
          if (hnode->l == node)
            hnode->l = noder;
          else
            break;
        }
      }
    }
  }

 protected:
  Node *SuccessorI(size_t x) const {
    if (IsEmpty()) return empty_node;
    auto hnode = Find(x);
    auto h = KeyToH(hnode->value);
    return h ? ((x >> (h - 1)) & 1) ? hnode->r->r : hnode->l : hnode->r;
  }

  Node *PredecessorI(size_t x) const {
    if (IsEmpty()) return empty_node;
    auto hnode = Find(x);
    auto h = KeyToH(hnode->value);
    return h ? ((x >> (h - 1)) & 1) ? hnode->r : hnode->l->l : hnode->l;
  }

 public:
  size_t Successor(size_t x) const { return SuccessorI(x)->value; }
  size_t Predecessor(size_t x) const { return PredecessorI(x)->value; }
};
}  // namespace fus
}  // namespace ds
