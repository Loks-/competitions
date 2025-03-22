#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/find_by_key_greater.h"
#include "common/binary_search_tree/base/find_by_key_less.h"
#include "common/data_structures/fixed_universe_successor/empty.h"

#include <algorithm>
#include <unordered_map>
#include <utility>

namespace ds {
namespace fus {
// Y-fast trie, use template classes for x-fast trie and groups tree
// Time and size will depend on template classes.
template <class TTXTrie, class TTGTree>
class YFastTrieProxy {
 public:
  using TXTrie = TTXTrie;
  using TGTree = TTGTree;
  using TGNode = typename TGTree::TNode;

 protected:
  class Group {
   public:
    size_t size;
    size_t min_value, max_value;
    TGNode* root;
  };

 protected:
  TXTrie xtrie;
  TGTree gtree;
  std::unordered_map<size_t, Group> groups;

  size_t size, usize;
  size_t group_min_size, group_max_size;

 protected:
  std::pair<size_t, size_t> Find2(size_t x) const {
    auto x2 = xtrie.Successor(x);
    auto x1 = xtrie.PredecessorI(x1);
    return {x1, x2};
  }

 public:
  YFastTrieProxy() { Clear(); }

  YFastTrieProxy(size_t u) { Init(u); }

  void Clear() {
    xtrie.Clear();
    gtree.Clear();
    groups.clear();
    size = 0;
  }
  void Init(size_t u) {
    Clear();
    usize = u;
    group_min_size = std::max(numeric::ULog2(uint64_t(u)) / 4u, 1u);
    group_max_size = 8 * group_min_size;
  }

  bool IsEmpty() const { return size == 0; }
  size_t Size() const { return size; }

  bool HasKey(size_t x) const {
    if (xtrie.HasKey(x)) return true;
    auto p = Find2(x);
    if (p.first != kEmpty) {
      auto it1 = groups.find(p.first);
      assert(it1 != groups.end());
      if (x <= it1->max_value) {
        return gtree.FindByKey(it1->root, x);
      }
    }
    if (p.second != kEmpty) {
      auto it2 = groups.find(p.second);
      assert(it2 != groups.end());
      if (x >= it2->min_value) {
        return gtree.FindByKey(it2->root, x);
      }
    }
    return false;
  }

  size_t Min() const {
    if (IsEmpty()) return kEmpty;
    auto x = xtrie.Min();
    auto it = groups.find(x);
    assert(it != groups.end());
    return it->min_value;
  }

  size_t Max() const {
    if (IsEmpty()) return kEmpty;
    auto x = xtrie.Maxn();
    auto it = groups.find(x);
    assert(it != groups.end());
    return it->max_value;
  }

 public:
  void Insert(size_t x) {
    if (xtrie.HasKey(x)) return;
    auto node = gtree.NewNode(x);
    if (IsEmpty()) {
      ++size;
      xtrie.Insert(x);
      auto& g = groups[x];
      g.size = 1;
      g.min_value = g.max_value = x;
      g.root = node;
      return;
    }
    auto p = Find2(x);
    assert(p.first != p.second);
    // if (p.first == Empty)
  }
  // void Insert(size_t x) {
  //   if (HasKey(x)) return;
  //   if (++size == 1) return InsertFirstNode(x);
  //   auto h1 = FindH(x);
  //   auto snode = vm[h1][x >> h1];
  //   auto sx = snode->value;
  //   auto noder = (x < sx) ? snode : snode->r;
  //   auto nodel = noder->l;
  //   auto node = node_manager.New();
  //   node->value = x;
  //   node->l = nodel;
  //   node->r = noder;
  //   nodel->r = node;
  //   noder->l = node;
  //   lm[x] = node;
  //   auto h0 = numeric::HighestBit(uint64_t(x ^ sx));
  //   auto xh = x >> h0;
  //   vm[h0][xh] = node;
  //   if (vm[h0].find(sx >> h0) == vm[h0].end()) vm[h0][sx >> h0] = snode;
  //   if (x < sx) {
  //     for (++h0; h0 < h1; ++h0, xh >>= 1)
  //       vm[h0][xh >> 1] = (xh & 1) ? node : snode;
  //     for (; h1 <= maxh; ++h1, xh >>= 1) {
  //       if ((xh & 1) == 1) {
  //         if (vm[h1][xh >> 1] == snode)
  //           vm[h1][xh >> 1] = node;
  //         else
  //           break;
  //       }
  //     }
  //   } else {
  //     for (++h0; h0 < h1; ++h0, xh >>= 1)
  //       vm[h0][xh >> 1] = (xh & 1) ? snode : node;
  //     for (; h1 <= maxh; ++h1, xh >>= 1) {
  //       if ((xh & 1) == 0) {
  //         if (vm[h1][xh >> 1] == snode)
  //           vm[h1][xh >> 1] = node;
  //         else
  //           break;
  //       }
  //     }
  //   }
  // }

  // void Delete(size_t x) {
  //   auto it = lm.find(x);
  //   if (it == lm.end()) return;       // No element to remove
  //   if (--size == 0) return Clear();  // Last element
  //   auto node = it->second;
  //   auto nodel = node->l, noder = node->r;
  //   nodel->r = noder;
  //   noder->l = nodel;
  //   lm.erase(it);
  //   node_manager.Release(node);
  //   auto h1 = FindH(x);
  //   auto xh = x >> h1;
  //   for (; vm[h1].find(xh ^ 1) == vm[h1].end(); ++h1, xh >>= 1)
  //     vm[h1].erase(xh);
  //   vm[h1].erase(xh);
  //   if (xh & 1) {
  //     xh >>= 1;
  //     vm[++h1][xh] = nodel;
  //     for (++h1; h1 <= maxh; ++h1, xh >>= 1) {
  //       if (~xh & 1) {
  //         if (vm[h1][xh >> 1] == node) {
  //           vm[h1][xh >> 1] = nodel;
  //         } else {
  //           break;
  //         }
  //       }
  //     }
  //   } else {
  //     xh >>= 1;
  //     vm[++h1][xh] = noder;
  //     for (++h1; h1 <= maxh; ++h1, xh >>= 1) {
  //       if (xh & 1) {
  //         if (vm[h1][xh >> 1] == node) {
  //           vm[h1][xh >> 1] = noder;
  //         } else {
  //           break;
  //         }
  //       }
  //     }
  //   }
  // }

 public:
  size_t Successor(size_t x) const {
    if (xtrie.Find(x)) {
      auto it = groups.find(x);
      assert(it != groups.end());
      if (it->max_value > x) {
        auto node = bst::base::FindByKeyGreater(it->root, x + 1);
        assert(node);
        return node->key;
      } else {
        auto xs = xtrie.Successor(x);
        if (xs == kEmpty) return xs;
        auto its = groups.find(xs);
        assert(its != groups.end());
        return its->min_value;
      }
    } else {
      auto p = Find2(x);
      if (p.first != kEmpty) {
        auto it1 = groups.find(p.first);
        assert(it1 != groups.end());
        if (x < it1->max_value) {
          auto node = bst::base::FindByKeyGreater(it1->root, x + 1);
          assert(node);
          return node->key;
        }
      }
      if (p.second != kEmpty) {
        auto it2 = groups.find(p.second);
        assert(it2 != groups.end());
        if (x < it2->min_value) return it2->min_value;
        if (x < it2->max_value) {
          auto node = bst::base::FindByKeyGreater(it2->root, x + 1);
          assert(node);
          return node->key;
        }
      }
      return kEmpty;
    }
  }

  size_t Predecessor(size_t x) const {
    if (xtrie.Find(x)) {
      auto it = groups.find(x);
      assert(it != groups.end());
      if (it->min_value < x) {
        auto node = bst::base::FindByKeyLess(it->root, x - 1);
        assert(node);
        return node->key;
      } else {
        auto xs = xtrie.Predecessor(x);
        if (xs == kEmpty) return xs;
        auto its = groups.find(xs);
        assert(its != groups.end());
        return its->max_value;
      }
    } else {
      auto p = Find2(x);
      if (p.second != kEmpty) {
        auto it2 = groups.find(p.second);
        assert(it2 != groups.end());
        if (x > it2->min_value) {
          auto node = bst::base::FindByKeyLess(it2->root, x - 1);
          assert(node);
          return node->key;
        }
      }
      if (p.first != kEmpty) {
        auto it1 = groups.find(p.first);
        assert(it1 != groups.end());
        if (x > it1->max_value) return it1->max_value;
        if (x > it1->min_value) {
          auto node = bst::base::FindByKeyLess(it1->root, x - 1);
          assert(node);
          return node->key;
        }
      }
      return kEmpty;
    }
  }
};
}  // namespace fus
}  // namespace ds
