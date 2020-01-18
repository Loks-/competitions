#pragma once

#include "common/calculus/ext_polynomial/term_bases/type.h"
#include <memory>
#include <string>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class TermBase {
 public:
  using TSelf = TermBase<TValue>;

  TermBase() {}
  virtual ~TermBase() {}

  virtual term_bases::Type GetType() const = 0;
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

  virtual bool IsOne() const { return GetType() == term_bases::Type::ONE; }
  virtual std::string ToString(const std::string& variable_name) const = 0;
};

template <class TValue>
using PTermBase = std::shared_ptr<TermBase<TValue>>;
}  // namespace ext_polynomial
}  // namespace calculus
