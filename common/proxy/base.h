#pragma once

template <class TTValue>
class ProxyBase {
 public:
  using TValue = TTValue;
  using TSelf = ProxyBase<TValue>;

 protected:
  TValue data;

 public:
  ProxyBase(const TValue& _data = TValue()) : data(_data) {}
  const TValue& Get() const { return data; }
  TValue& Get() { return data; }
  void Set(const TValue& _data) { data = _data; }

  bool operator<(const TSelf& r) const { return data < r.data; }
};
