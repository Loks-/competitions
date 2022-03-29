#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/first_bit.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <memory>
#include <unordered_map>

namespace ds {
namespace fus {
// Van Emde Boas tree with hash table for children
// N - number of elements, M - max value in set, m = log(M)
// Space - O(N log M)
// Insert / Remove - O(log log M)
// Next / Prev - O(log log M)
// Min / Max - O(1)
class VanEmdeBoasTreeHashTableTemp {
 public:
  using TValue = uint64_t;
  using TSelf = VanEmdeBoasTreeHashTableTemp;
  using PSelf = std::shared_ptr<TSelf>;

 protected:
  unsigned m, mh;
  size_t mask_low;
  uint64_t aux_mask;
  PSelf aux_tree;
  size_t size;
  size_t min_value, max_value;
  std::unordered_map<size_t, PSelf> children;

 protected:
  bool Flat() const { return m <= 6; }

  static PSelf Make(unsigned _m) { return std::make_shared<TSelf>(_m); }

 public:
  VanEmdeBoasTreeHashTableTemp() {}
  VanEmdeBoasTreeHashTableTemp(unsigned _m) { InitL(_m); }

  void InitL(unsigned _m) {
    m = _m;
    mh = m / 2;
    mask_low = (1ull << mh) - 1;
    aux_mask = 0;
    if (!Flat()) aux_tree = Make(m - mh);
    size = 0;
    min_value = max_value = Empty;
  }

  void Init(size_t u) { InitL(u ? numeric::ULog2(u - 1) + 1 : 1); }

  size_t Size() const { return size; }

  size_t Min() const { return min_value; }
  size_t Max() const { return max_value; }

  bool HasKey(size_t x) const {
    if (Flat()) {
      return aux_mask & (1ull << x);
    } else {
      size_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      return (it != children.end()) ? it->second->HasKey(x2) : false;
    }
  }

  void Insert(size_t x) {
    if (!Size()) {
      min_value = max_value = x;
    } else {
      min_value = std::min(min_value, x);
      max_value = std::max(max_value, x);
    }
    if (Flat()) {
      aux_mask |= (1ull << x);
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if (it == children.end()) {
        aux_tree->Insert(x1);
        children.insert({x1, Make(mh)});
        it = children.find(x1);
      }
      it->second->Insert(x2);
    }
    ++size;
    assert(Flat() || (children.size() == aux_tree->Size()));
  }

  void Delete(size_t x) {
    if (Flat()) {
      aux_mask &= ~(1ull << x);
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      assert(it != children.end());
      if (it->second->Size() == 1) {
        children.erase(it);
        aux_tree->Delete(x1);
      } else {
        it->second->Delete(x2);
      }
    }
    --size;
    assert(Flat() || (children.size() == aux_tree->Size()));
    if (!Size()) return;
    if (x == min_value) {
      if (Flat()) {
        min_value = numeric::Lowest0Bits(aux_mask);
      } else {
        size_t x1 = aux_tree->Min();
        min_value = (x1 << mh) + children[x1]->Min();
      }
    }
    if (x == max_value) {
      if (Flat()) {
        max_value = 63 - numeric::Highest0Bits(aux_mask);
      } else {
        size_t x1 = aux_tree->Max();
        max_value = (x1 << mh) + children[x1]->Max();
      }
    }
  }

  size_t Successor(size_t x) {
    if ((x >= max_value) || (max_value == Empty)) return Empty;
    if (Flat()) {
      return numeric::Lowest0Bits(aux_mask &
                                  ~uint64_t(((1ull << (x + 1)) - 1)));
    } else {
      size_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if ((it != children.end()) && (it->second->Max() > x2))
        return (x1 << mh) + it->second->Successor(x2);
      x1 = aux_tree->Successor(x1);
      return (x1 << mh) + children[x1]->Min();
    }
  }

  size_t Predecessor(size_t x) {
    if (x <= min_value) return Empty;
    if (Flat()) {
      return 63 - numeric::Highest0Bits(aux_mask & uint64_t(((1ull << x) - 1)));
    } else {
      size_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if ((it != children.end()) && (it->second->Min() < x2))
        return (x1 << mh) + it->second->Predecessor(x2);
      x1 = aux_tree->Predecessor(x1);
      return (x1 << mh) + children[x1]->Max();
    }
  }
};
}  // namespace fus
}  // namespace ds
