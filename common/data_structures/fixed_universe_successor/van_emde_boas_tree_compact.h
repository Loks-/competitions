#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/u64_set.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <memory>
#include <unordered_map>

namespace ds {
namespace fus {
// Van Emde Boas tree (hash table for children). Use U64Set class on low level.
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log log U)
// HasKey      -- O(log log U)
// Delete      -- O(log log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
class VanEmdeBoasTreeCompact {
 public:
  using TSelf = VanEmdeBoasTreeCompact;
  using PSelf = std::shared_ptr<TSelf>;

 protected:
  unsigned m, mh;
  size_t mask_low;

  U64Set leaf;
  PSelf aux_tree;
  std::unordered_map<size_t, PSelf> children;

  size_t min_value, max_value;

 protected:
  bool Flat() const { return m <= 6; }

  static PSelf Make(unsigned _m, size_t x) {
    return std::make_shared<TSelf>(_m, x);
  }

 public:
  VanEmdeBoasTreeCompact() {}
  VanEmdeBoasTreeCompact(unsigned _m, size_t x) { InitL(_m, x); }

 protected:
  void InitL(unsigned _m, size_t x) {
    m = _m;
    if (!Flat()) {
      mh = m / 2;
      mask_low = (1ull << mh) - 1;
      aux_tree = 0;
      min_value = max_value = x;
    } else if (x != Empty) {
      leaf.Insert(x);
    }
  }

 public:
  void Init(size_t u) { InitL(u ? numeric::ULog2(u - 1) + 1 : 1, Empty); }

  bool IsEmpty() const {
    return Flat() ? leaf.IsEmpty() : (min_value == Empty);
  }

  size_t Min() const { return Flat() ? leaf.Min() : min_value; }
  size_t Max() const { return Flat() ? leaf.Max() : max_value; }

  bool HasKey(size_t x) const {
    if (Flat()) return leaf.HasKey(x);
    if (x < min_value) return false;
    if (x == min_value) return true;
    auto it = children.find(x >> mh);
    return (it == children.end()) ? false : it->second->HasKey(x & mask_low);
  }

  void Insert(size_t x) {
    if (Flat()) return leaf.Insert(x);
    if (min_value == Empty) {
      min_value = max_value = x;
    } else {
      if (x < min_value)
        std::swap(min_value, x);
      else
        max_value = std::max(max_value, x);
      size_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if (it == children.end()) {
        if (aux_tree)
          aux_tree->Insert(x1);
        else
          aux_tree = Make(m - mh, x1);
        children[x1] = Make(mh, x2);
      } else {
        children[x1]->Insert(x2);
      }
    }
  }

  void Delete(size_t x) {
    if (Flat()) return leaf.Delete(x);
    if (x == min_value) {
      if (min_value == max_value) {
        min_value = max_value = Empty;
        return;
      }
      assert(aux_tree);
      auto x1 = aux_tree->Min();
      x = (x1 << mh) + children[x1]->Min();
      min_value = x;
    }
    auto x1 = (x >> mh), x2 = (x & mask_low);
    auto it1 = children.find(x1);
    if (it1 == children.end()) return;  // No element to remove
    it1->second->Delete(x2);
    if (it1->second->IsEmpty()) {
      assert(aux_tree);
      aux_tree->Delete(x1);
      if (aux_tree->IsEmpty()) {
        aux_tree = nullptr;
      }
      children.erase(x1);
    }
    if (x == max_value) {
      if (!aux_tree || aux_tree->IsEmpty()) {
        max_value = min_value;
      } else {
        auto y1 = aux_tree->Max();
        max_value = (y1 << mh) + children[y1]->Max();
      }
    }
  }

  size_t Successor(size_t x) {
    if (Flat()) return leaf.Successor(x);
    if (x < min_value) return min_value;
    if (x >= max_value) return Empty;
    size_t x1 = (x >> mh), x2 = (x & mask_low);
    auto it1 = children.find(x1);
    if ((it1 != children.end()) && (x2 < it1->second->Max()))
      return (x1 << mh) + it1->second->Successor(x2);
    assert(aux_tree);
    x1 = aux_tree->Successor(x1);
    return (x1 << mh) + children[x1]->Min();
  }

  size_t Predecessor(size_t x) {
    if (Flat()) return leaf.Predecessor(x);
    if (x > max_value) return max_value;
    if (x <= min_value) return Empty;
    size_t x1 = (x >> mh), x2 = (x & mask_low);
    auto it1 = children.find(x1);
    if ((it1 != children.end()) && (x2 > it1->second->Min()))
      return (x1 << mh) + it1->second->Predecessor(x2);
    assert(aux_tree);
    x1 = aux_tree->Predecessor(x1);
    return (x1 == Empty) ? min_value : (x1 << mh) + children[x1]->Max();
  }
};
}  // namespace fus
}  // namespace ds
