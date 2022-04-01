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
 public:
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
    return (~((1ull << h) - 1) & x) + (uint64_t(h) << 58);
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
    assert(r);
    return r;
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
    unsigned h = 1;
    for (;; ++h) {
      auto key = HXToKey(h, x);
      auto it = m.find(key);
      if (it == m.end()) {
        auto hnode = node_manager.New();
        hnode->value = key;
        if ((x >> (h - 1)) & 1) {
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
        if ((x >> (h - 1)) & 1) {
          hnode->r = last_node;
        } else {
          hnode->l = last_node;
        }
        break;
      }
    }
    // Temp slow version
    for (++h; h < maxh; ++h) {
      auto key = HXToKey(h, x);
      auto hnode = m[key];
      assert(hnode);
      if (KeyToH(hnode->l->value) == 0) {
        if (hnode->l->value > x) hnode->l = node;
      }
      if (KeyToH(hnode->r->value) == 0) {
        if (hnode->r->value < x) hnode->r = node;
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
    unsigned h = 1;
    for (;; ++h) {
      auto hkey = HXToKey(h, x), skey = HXToKey(h - 1, x ^ (1ull << (h - 1)));
      auto hnode = m[hkey];
      auto sit = m.find(skey);
      if (sit == m.end()) {
        m.erase(hkey);
      } else {
        if ((x >> (h - 1)) & 1) {
          hnode->r = nodel;
        } else {
          hnode->l = noder;
        }
        break;
      }
    }
    // Temp slow version
    for (++h; h < maxh; ++h) {
      auto hkey = HXToKey(h, x);
      auto hnode = m[hkey];
      assert(hnode);
      if (hnode->l == node) hnode->l = noder;
      if (hnode->r == node) hnode->r = nodel;
    }
  }

 protected:
  Node *SuccessorI(size_t x) {
    if (IsEmpty()) return empty_node;
    auto hnode = Find(x);
    auto h = KeyToH(hnode->value);
    return h ? ((x >> (h - 1)) & 1) ? hnode->r->r : hnode->l : hnode->r;
  }

  Node *PredecessorI(size_t x) {
    if (IsEmpty()) return empty_node;
    auto hnode = Find(x);
    auto h = KeyToH(hnode->value);
    return h ? ((x >> (h - 1)) & 1) ? hnode->r : hnode->l->l : hnode->l;
  }

 public:
  size_t Successor(size_t x) { return SuccessorI(x)->value; }
  size_t Predecessor(size_t x) { return PredecessorI(x)->value; }
};
}  // namespace fus
}  // namespace ds
