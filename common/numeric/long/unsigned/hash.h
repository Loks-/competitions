#pragma once

#include "common/hash/base.h"
#include "common/hash/vector.h"
#include "common/numeric/long/unsigned.h"

namespace nhash {
template <class Policy>
class Hash<Policy, numeric::nlong::Unsigned> {
 public:
  constexpr size_t operator()(const numeric::nlong::Unsigned& u) const {
    return Hash<Policy, numeric::nlong::Unsigned::TData>{}(u.Data());
  }
};
}  // namespace nhash
