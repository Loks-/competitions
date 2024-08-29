#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
#include "common/numeric/utils/pow.h"

#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class TermBase;

template <class TValue>
using PTermBase = std::shared_ptr<TermBase<TValue>>;

namespace term_bases {
template <class TValue>
inline PTermBase<TValue> MakeOne();
}  // namespace term_bases

template <class TValue>
class TermBase {
 public:
  using TSelf = TermBase<TValue>;
  using PSelf = PTermBase<TValue>;

  constexpr TermBase() {}

  virtual constexpr ~TermBase() {}

  virtual constexpr term_bases::Type GetType() const = 0;

  virtual constexpr bool IsOne() const {
    return GetType() == term_bases::Type::ONE;
  }

  virtual constexpr TValue BaseGet(const TValue&) const {
    assert(false);
    return TValue(0);
  }

  virtual constexpr TValue GetXPower(const TValue& x, int power) const {
    return PowS(x, power);
  }

  virtual constexpr TValue Get(const TValue& x, int power) const {
    return GetXPower(x, power) * BaseGet(x);
  }

  virtual constexpr bool IsBaseFinite(const TValue&) const { return true; }

  virtual constexpr bool IsFinite(const TValue& x, int power) const {
    return IsBaseFinite(x) && ((x != TValue(0)) || (power >= 0));
  }

  virtual constexpr bool SameType(const TSelf& r) const {
    return GetType() == r.GetType();
  }

  virtual constexpr bool SameTypeLess(const TSelf&) const { return false; }

  virtual constexpr bool SameTypeEqual(const TSelf&) const { return true; }

  virtual constexpr bool operator<(const TSelf& r) const {
    return SameType(r) ? SameTypeLess(r) : (GetType() < r.GetType());
  }

  virtual constexpr bool operator==(const TSelf& r) const {
    return SameType(r) && SameTypeEqual(r);
  }

  virtual constexpr bool IsMultiplicable(const TSelf& r) const {
    return IsOne() || r.IsOne();
  }

  virtual PSelf Multiplication(const PSelf& pl, const PSelf& pr) const {
    // assert(pl.get() == this);
    if (pl->IsOne()) return pr;
    if (pr->IsOne()) return pl;
    assert(false);
    return nullptr;
  }

  virtual constexpr bool IsDivisible(const TSelf& r) const {
    return r.IsOne() || (*this == r);
  }

  virtual PSelf Division(const PSelf& pl, const PSelf& pr) const {
    // assert(pl.get() == this);
    if (pr->IsOne()) return pl;
    if (*pl == *pr) return term_bases::MakeOne<TValue>();
    assert(false);
    return nullptr;
  }

  virtual constexpr std::string ToString(
      const std::string& variable_name) const = 0;
};
}  // namespace ext_polynomial
}  // namespace calculus
