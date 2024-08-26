#pragma once

#include "common/proxy/base.h"

namespace proxy {
template <class TValue>
class Or : public Base<TValue> {
 public:
  using TBase = Base<TValue>;
  using TSelf = Or<TValue>;

 public:
  constexpr Or() : TBase() {}

  constexpr explicit Or(const TValue& _data) : TBase(_data) {}

  constexpr TSelf& operator+=(const TSelf& r) {
    TBase::data |= r.data;
    return *this;
  }

  constexpr TSelf operator+(const TSelf& r) {
    return TSelf(TBase::data | r.data);
  }
};
}  // namespace proxy
