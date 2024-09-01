#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/data_structures/fixed_universe_successor/fixed_length_set_6b.h"

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
template <unsigned nbits>
class VanEmdeBoasTreeCompactStaticSize;

template <>
class VanEmdeBoasTreeCompactStaticSize<1> : public FLSetB6 {};

template <>
class VanEmdeBoasTreeCompactStaticSize<2> : public FLSetB6 {};

template <>
class VanEmdeBoasTreeCompactStaticSize<3> : public FLSetB6 {};

template <>
class VanEmdeBoasTreeCompactStaticSize<4> : public FLSetB6 {};

template <>
class VanEmdeBoasTreeCompactStaticSize<5> : public FLSetB6 {};

template <>
class VanEmdeBoasTreeCompactStaticSize<6> : public FLSetB6 {};

template <unsigned nbits_high, unsigned nbits_low>
class VanEmdeBoasTreeCompactStaticSizeI {
 protected:
  static constexpr size_t mask_low = (size_t(1) << nbits_low) - 1;

 protected:
  std::shared_ptr<VanEmdeBoasTreeCompactStaticSize<nbits_high>> aux_tree;
  std::unordered_map<
      size_t, std::shared_ptr<VanEmdeBoasTreeCompactStaticSize<nbits_low>>>
      children;

  size_t min_value, max_value;

 protected:
  template <unsigned nbits>
  static std::shared_ptr<VanEmdeBoasTreeCompactStaticSize<nbits>> Make(
      size_t x) {
    auto p = std::make_shared<VanEmdeBoasTreeCompactStaticSize<nbits>>();
    if (x != Empty) p->Insert(x);
    return p;
  }

 public:
  VanEmdeBoasTreeCompactStaticSizeI() { Clear(); }

  void Clear() {
    aux_tree = nullptr;
    children.clear();
    min_value = max_value = Empty;
  }

 public:
  void Init(size_t) { Clear(); }

  bool IsEmpty() const { return (min_value == Empty); }

  size_t Min() const { return min_value; }

  size_t Max() const { return max_value; }

  bool HasKey(size_t x) const {
    if (x < min_value) return false;
    if (x == min_value) return true;
    const auto it = children.find(x >> nbits_low);
    return (it == children.end()) ? false : it->second->HasKey(x & mask_low);
  }

  void Insert(size_t x) {
    if (min_value == Empty) {
      min_value = max_value = x;
    } else {
      if (x < min_value)
        std::swap(min_value, x);
      else
        max_value = std::max(max_value, x);
      const size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
      auto it = children.find(x1);
      if (it == children.end()) {
        if (aux_tree)
          aux_tree->Insert(x1);
        else
          aux_tree = Make<nbits_high>(x1);
        children[x1] = Make<nbits_low>(x2);
      } else {
        it->second->Insert(x2);
      }
    }
  }

  void Delete(size_t x) {
    if (x == min_value) {
      if (min_value == max_value) {
        min_value = max_value = Empty;
        return;
      }
      assert(aux_tree);
      const auto x1 = aux_tree->Min();
      x = (x1 << nbits_low) + children[x1]->Min();
      min_value = x;
    }
    const auto x1 = (x >> nbits_low), x2 = (x & mask_low);
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
        const auto y1 = aux_tree->Max();
        max_value = (y1 << nbits_low) + children[y1]->Max();
      }
    }
  }

  size_t Successor(size_t x) const {
    if (x < min_value) return min_value;
    if (x >= max_value) return Empty;
    size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
    auto it1 = children.find(x1);
    if ((it1 != children.end()) && (x2 < it1->second->Max()))
      return (x1 << nbits_low) + it1->second->Successor(x2);
    assert(aux_tree);
    x1 = aux_tree->Successor(x1);
    it1 = children.find(x1);
    return (x1 << nbits_low) + it1->second->Min();
  }

  size_t Predecessor(size_t x) const {
    if (x > max_value) return max_value;
    if (x <= min_value) return Empty;
    size_t x1 = (x >> nbits_low), x2 = (x & mask_low);
    auto it1 = children.find(x1);
    if ((it1 != children.end()) && (x2 > it1->second->Min()))
      return (x1 << nbits_low) + it1->second->Predecessor(x2);
    assert(aux_tree);
    x1 = aux_tree->Predecessor(x1);
    if (x1 == Empty) return min_value;
    it1 = children.find(x1);
    return (x1 << nbits_low) + it1->second->Max();
  }
};

template <unsigned dim>
class VanEmdeBoasTreeCompactStaticSize
    : public VanEmdeBoasTreeCompactStaticSizeI<dim - 6 * ((dim + 5) / 12),
                                               6 * ((dim + 5) / 12)> {};
}  // namespace fus
}  // namespace ds
