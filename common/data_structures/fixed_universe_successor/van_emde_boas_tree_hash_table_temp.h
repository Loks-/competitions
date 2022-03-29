#pragma once

#include "common/base.h"
#include "common/numeric/bits/first_bit.h"

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
  uint64_t mask_low;
  uint64_t aux_mask;
  PSelf aux_tree;
  uint64_t size;
  uint64_t min_value, max_value;
  std::unordered_map<uint64_t, PSelf> children;

 protected:
  bool Flat() const { return m <= 6; }

  static PSelf Make(unsigned _m) { return std::make_shared<TSelf>(_m); }

 public:
  VanEmdeBoasTreeHashTableTemp(unsigned _m) {
    m = _m;
    mh = m / 2;
    mask_low = (1ull << mh) - 1;
    aux_mask = 0;
    if (!Flat()) aux_tree = Make(m - mh);
    size = 0;
  }

  bool Empty() const { return size == 0; }
  unsigned Size() const { return size; }
  uint64_t MinValue() const { return min_value; }
  uint64_t MaxValue() const { return max_value; }

  bool HasKey(uint64_t x) const {
    if (Flat()) {
      return aux_mask & (1ull << x);
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      return (it != children.end()) ? it->second->HasKey(x2) : false;
    }
  }

  void Insert(uint64_t x) {
    if (Empty()) {
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

  void Remove(uint64_t x) {
    if (Flat()) {
      aux_mask &= ~(1ull << x);
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      assert(it != children.end());
      if (it->second->Size() == 1) {
        children.erase(it);
        aux_tree->Remove(x1);
      } else {
        it->second->Remove(x2);
      }
    }
    --size;
    assert(Flat() || (children.size() == aux_tree->Size()));
    if (Empty()) return;
    if (x == min_value) {
      if (Flat()) {
        min_value = numeric::Lowest0Bits(aux_mask);
      } else {
        uint64_t x1 = aux_tree->MinValue();
        min_value = (x1 << mh) + children[x1]->MinValue();
      }
    }
    if (x == max_value) {
      if (Flat()) {
        max_value = 63 - numeric::Highest0Bits(aux_mask);
      } else {
        uint64_t x1 = aux_tree->MaxValue();
        max_value = (x1 << mh) + children[x1]->MaxValue();
      }
    }
  }

  uint64_t Next(uint64_t x) {
    if (x >= MaxValue()) return MaxValue();
    if (Flat()) {
      return numeric::Lowest0Bits(aux_mask &
                                  ~uint64_t(((1ull << (x + 1)) - 1)));
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if ((it != children.end()) && (it->second->MaxValue() > x2))
        return (x1 << mh) + it->second->Next(x2);
      x1 = aux_tree->Next(x1);
      return (x1 << mh) + children[x1]->MinValue();
    }
  }

  uint64_t Prev(uint64_t x) {
    if (x <= MinValue()) return MinValue();
    if (Flat()) {
      return 63 - numeric::Highest0Bits(aux_mask & uint64_t(((1ull << x) - 1)));
    } else {
      uint64_t x1 = (x >> mh), x2 = (x & mask_low);
      auto it = children.find(x1);
      if ((it != children.end()) && (it->second->MinValue() < x2))
        return (x1 << mh) + it->second->Prev(x2);
      x1 = aux_tree->Prev(x1);
      return (x1 << mh) + children[x1]->MaxValue();
    }
  }
};
}  // namespace fus
}  // namespace ds
