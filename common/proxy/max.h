#pragma once

#include "common/proxy/base.h"

#include <algorithm>
#include <limits>

namespace proxy {
template <class TValue>
class Max : public Base<TValue> {
 public:
  using TBase = Base<TValue>;
  using TSelf = Max<TValue>;

  Max() : TBase(std::numeric_limits<TValue>::lowest()) {}
  explicit Max(const TValue& _data) : TBase(_data) {}

  TSelf& operator+=(const TSelf& r) {
    TBase::data = std::max(TBase::data, r.data);
    return *this;
  }

  TSelf operator+(const TSelf& r) {
    return TSelf(std::max(TBase::data, r.data));
  }
};
}  // namespace proxy
