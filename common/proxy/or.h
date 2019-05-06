#pragma once

#include "common/proxy/base.h"

template <class TValue>
class ProxyOr : public ProxyBase<TValue> {
 public:
  using TBase = ProxyBase<TValue>;
  using TSelf = ProxyOr<TValue>;

  ProxyOr() : TBase() {}
  ProxyOr(const TValue& _data) : TBase(_data) {}

  TSelf& operator+=(const TSelf& r) {
    TBase::data |= r.data;
    return *this;
  }

  TSelf operator+(const TSelf& r) { return TSelf(TBase::data | r.data); }
};
