#pragma once

#include "common/hash/base.h"
#include "common/hash/pair.h"
#include "common/numeric/fraction.h"

namespace nhash {
template <class Policy, class T>
class Hash<Policy, numeric::Fraction<T>> {
 public:
  constexpr size_t operator()(const numeric::Fraction<T>& p) const {
    return Hash<Policy, std::pair<T, T>>{}({p.GetN(), p.GetD()});
  }
};
}  // namespace nhash
