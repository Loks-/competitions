#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/u64_set.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Static Van Emde Boas tree (full). Use U64Set class on low level.
// Memory      -- O(U)
// Init        -- O(U)
// Insert      -- O(log log U)
// HasKey      -- O(log log U)
// Delete      -- O(log log U)
// Min         -- O(1)
// Max         -- O(1)
// Successor   -- O(log log U)
// Predecessor -- O(log log U)
template <unsigned nbits>
class VanEmdeBoasTreeFullStaticSize;

template <>
class VanEmdeBoasTreeFullStaticSize<1> : public U64Set {};

template <>
class VanEmdeBoasTreeFullStaticSize<2> : public U64Set {};

template <>
class VanEmdeBoasTreeFullStaticSize<3> : public U64Set {};

template <>
class VanEmdeBoasTreeFullStaticSize<4> : public U64Set {};

template <>
class VanEmdeBoasTreeFullStaticSize<5> : public U64Set {};

template <>
class VanEmdeBoasTreeFullStaticSize<6> : public U64Set {};

template <unsigned nbits_high, unsigned nbits_low>
class VanEmdeBoasTreeFullStaticSizeI {
 protected:
  static const uint64_t mask_low = (1ull << nbits_low) - 1;
  VanEmdeBoasTreeFullStaticSize<nbits_high> aux_tree;
  std::vector<VanEmdeBoasTreeFullStaticSize<nbits_low>> children;

  size_t min_value, max_value;

 public:
  VanEmdeBoasTreeFullStaticSizeI() { Clear(); }
  void Init(size_t) { Clear(); }

  void Clear() {
    min_value = max_value = Empty;
    children.resize((1ull << nbits_high));
  }

  bool IsEmpty() const { return (min_value == Empty); }

  size_t Min() const { return min_value; }
  size_t Max() const { return max_value; }

  bool HasKey(size_t x) const {
    if (x < min_value) return false;
    if (x == min_value) return true;
    return children[x >> nbits_low].HasKey(x & mask_low);
  }

  void Insert(size_t x) {
    if (min_value == Empty) {
      min_value = max_value = x;
    } else {
      if (x < min_value)
        std::swap(min_value, x);
      else
        max_value = std::max(max_value, x);
      size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
      if (children[x1].IsEmpty()) {
        aux_tree.Insert(x1);
      }
      children[x1].Insert(x2);
    }
  }

  void Delete(size_t x) {
    if (x == min_value) {
      if (min_value == max_value) {
        min_value = max_value = Empty;
        return;
      }
      auto x1 = aux_tree.Min();
      x = (x1 << nbits_low) + children[x1].Min();
      min_value = x;
    }
    auto x1 = (x >> nbits_low), x2 = (x & mask_low);
    children[x1].Delete(x2);
    if (children[x1].IsEmpty()) aux_tree.Delete(x1);
    if (x == max_value) {
      if (aux_tree.IsEmpty()) {
        max_value = min_value;
      } else {
        auto y1 = aux_tree.Max();
        max_value = (y1 << nbits_low) + children[y1].Max();
      }
    }
  }

  size_t Successor(size_t x) {
    if (x < min_value) return min_value;
    if (x >= max_value) return Empty;
    size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
    if (!children[x1].IsEmpty() && (x2 < children[x1].Max()))
      return (x1 << nbits_low) + children[x1].Successor(x2);
    x1 = aux_tree.Successor(x1);
    return (x1 << nbits_low) + children[x1].Min();
  }

  size_t Predecessor(size_t x) {
    if (x > max_value) return max_value;
    if (x <= min_value) return Empty;
    size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
    if (x2 > children[x1].Min())
      return (x1 << nbits_low) + children[x1].Predecessor(x2);
    x1 = aux_tree.Predecessor(x1);
    return (x1 == Empty) ? min_value : (x1 << nbits_low) + children[x1].Max();
  }
};

template <>
class VanEmdeBoasTreeFullStaticSize<7>
    : public VanEmdeBoasTreeFullStaticSizeI<1, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<8>
    : public VanEmdeBoasTreeFullStaticSizeI<2, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<9>
    : public VanEmdeBoasTreeFullStaticSizeI<3, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<10>
    : public VanEmdeBoasTreeFullStaticSizeI<4, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<11>
    : public VanEmdeBoasTreeFullStaticSizeI<5, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<12>
    : public VanEmdeBoasTreeFullStaticSizeI<6, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<13>
    : public VanEmdeBoasTreeFullStaticSizeI<7, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<14>
    : public VanEmdeBoasTreeFullStaticSizeI<8, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<15>
    : public VanEmdeBoasTreeFullStaticSizeI<9, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<16>
    : public VanEmdeBoasTreeFullStaticSizeI<10, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<17>
    : public VanEmdeBoasTreeFullStaticSizeI<11, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<18>
    : public VanEmdeBoasTreeFullStaticSizeI<12, 6> {};

template <>
class VanEmdeBoasTreeFullStaticSize<19>
    : public VanEmdeBoasTreeFullStaticSizeI<7, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<20>
    : public VanEmdeBoasTreeFullStaticSizeI<8, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<21>
    : public VanEmdeBoasTreeFullStaticSizeI<9, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<22>
    : public VanEmdeBoasTreeFullStaticSizeI<10, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<23>
    : public VanEmdeBoasTreeFullStaticSizeI<11, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<24>
    : public VanEmdeBoasTreeFullStaticSizeI<12, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<25>
    : public VanEmdeBoasTreeFullStaticSizeI<13, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<26>
    : public VanEmdeBoasTreeFullStaticSizeI<14, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<27>
    : public VanEmdeBoasTreeFullStaticSizeI<15, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<28>
    : public VanEmdeBoasTreeFullStaticSizeI<16, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<29>
    : public VanEmdeBoasTreeFullStaticSizeI<17, 12> {};

template <>
class VanEmdeBoasTreeFullStaticSize<30>
    : public VanEmdeBoasTreeFullStaticSizeI<18, 12> {};
}  // namespace fus
}  // namespace ds
