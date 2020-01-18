#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/term_bases/type.h"
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

  TermBase() {}
  virtual ~TermBase() {}

  virtual term_bases::Type GetType() const = 0;
  virtual bool IsOne() const { return GetType() == term_bases::Type::ONE; }

  virtual TValue Get(const TValue& x) const = 0;

  virtual bool SameType(const TSelf& r) const {
    return GetType() == r.GetType();
  }

  virtual bool SameTypeLess(const TSelf& r) const { return false; }
  virtual bool SameTypeEqual(const TSelf& r) const { return true; }

  virtual bool operator<(const TSelf& r) const {
    return SameType(r) ? SameTypeLess(r) : (GetType() < r.GetType());
  }

  virtual bool operator==(const TSelf& r) const {
    return SameType(r) && SameTypeEqual(r);
  }

  virtual bool IsMultiplicable(const TSelf& r) const {
    return IsOne() || r.IsOne();
  }

  virtual PSelf Multiplication(const PSelf& pl, const PSelf& pr) const {
    assert(pl.get() == this);
    if (pl->IsOne()) return pr;
    if (pr->IsOne()) return pl;
    assert(false);
    return nullptr;
  }

  virtual bool IsDivisible(const TSelf& r) const {
    return r.IsOne() || (*this == r);
  }

  virtual PSelf Division(const PSelf& pl, const PSelf& pr) const {
    assert(pl.get() == this);
    if (pr->IsOne()) return pl;
    if (*pl == *pr) return term_bases::MakeOne<TValue>();
    assert(false);
    return nullptr;
  }

  virtual std::string ToString(const std::string& variable_name) const = 0;
};
}  // namespace ext_polynomial
}  // namespace calculus
