#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/u64_set.h"
#include "common/numeric/bits/first_bit.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace ds {
namespace fus {
// Van Emde Boas tree (full). Use U64Set class on low level.
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(log log U)
// HasKey      -- O(log log U)
// Delete      -- O(log log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
class VanEmdeBoasTreeFull {
 public:
  using TSelf = VanEmdeBoasTreeFull;
  using PSelf = std::shared_ptr<TSelf>;

 protected:
  unsigned m, mh;
  size_t mask_low;

  U64Set leaf;
  uint64_t aux_mask;
  PSelf aux_tree;
  std::vector<TSelf> children;

  size_t min_value, max_value;

 protected:
  bool Flat() const { return m <= 6; }

  static PSelf Make(unsigned _m) { return std::make_shared<TSelf>(_m); }

 public:
  VanEmdeBoasTreeFull() {}
  VanEmdeBoasTreeFull(unsigned _m) { InitL(_m); }

 protected:
  void InitL(unsigned _m) {
    m = _m;
    if (!Flat()) {
      mh = m / 2;
      mask_low = (1ull << mh) - 1;
      aux_mask = 0;
      aux_tree = Make(m - mh);
      children.resize((1ull << (m - mh)));
      for (auto& c : children) c.InitL(mh);
      min_value = max_value = Empty;
    }
  }

 public:
  void Init(size_t u) { InitL(u ? numeric::ULog2(u - 1) + 1 : 1); }

  bool IsEmpty() const {
    return Flat() ? leaf.IsEmpty() : (min_value == Empty);
  }

  size_t Min() const { return Flat() ? leaf.Min() : min_value; }
  size_t Max() const { return Flat() ? leaf.Max() : max_value; }

  bool HasKey(size_t x) const {
    if (Flat()) return leaf.HasKey(x);
    if (x < min_value) return false;
    if (x == min_value) return true;
    return children[x >> mh].HasKey(x & mask_low);
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
      if (children[x1].IsEmpty()) {
        aux_tree->Insert(x1);
      }
      children[x1].Insert(x2);
    }
  }

  void Delete(size_t x) {
    if (Flat()) return leaf.Delete(x);
    if (x == min_value) {
      if (min_value == max_value) {
        min_value = max_value = Empty;
        return;
      }
      auto x1 = aux_tree->Min();
      x = (x1 << mh) + children[x1].Min();
      min_value = x;
    }
    auto x1 = (x >> mh), x2 = (x & mask_low);
    children[x1].Delete(x2);
    if (children[x1].IsEmpty()) aux_tree->Delete(x1);
    if (x == max_value) {
      if (aux_tree->IsEmpty()) {
        max_value = min_value;
      } else {
        auto y1 = aux_tree->Max();
        max_value = (y1 << mh) + children[y1].Max();
      }
    }
  }

  size_t Successor(size_t x) {
    if (Flat()) return leaf.Successor(x);
    if (x < min_value) return min_value;
    if (x >= max_value) return Empty;
    size_t x1 = (x >> mh), x2 = (x & mask_low);
    if (!children[x1].IsEmpty() && (x2 < children[x1].Max()))
      return (x1 << mh) + children[x1].Successor(x2);
    x1 = aux_tree->Successor(x1);
    return (x1 << mh) + children[x1].Min();
  }

  size_t Predecessor(size_t x) {
    if (Flat()) return leaf.Predecessor(x);
    if (x > max_value) return max_value;
    if (x <= min_value) return Empty;
    size_t x1 = (x >> mh), x2 = (x & mask_low);
    if (x2 > children[x1].Min())
      return (x1 << mh) + children[x1].Predecessor(x2);
    x1 = aux_tree->Predecessor(x1);
    return (x1 == Empty) ? min_value : (x1 << mh) + children[x1].Max();
  }
};
}  // namespace fus
}  // namespace ds
