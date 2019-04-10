#pragma once

#include "common/proxy/base.h"
#include <algorithm>
#include <limits>

template <class TValue>
class ProxyMax : public ProxyBase<TValue> {
 public:
  using TBase = ProxyBase<TValue>;
  using TSelf = ProxyMax<TValue>;

  ProxyMax() : TBase(std::numeric_limits<TValue>::lowest()) {}
  ProxyMax(const TValue& _data) : TBase(_data) {}

  TSelf& operator+=(const TSelf& r) {
    TBase::data = std::max(TBase::data, r.data);
    return *this;
  }

  TSelf operator+(const TSelf& r) {
    return TSelf(std::max(TBase::data, r.data));
  }
};
