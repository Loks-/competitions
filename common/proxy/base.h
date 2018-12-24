#pragma once

template <class TValue>
class ProxyBase {
 protected:
  TValue data;

 public:
  ProxyBase(const TValue& _data = TValue()) : data(_data) {}
  const TValue& Get() const { return data; }
  TValue& Get() { return data; }
  void Set(const TValue& _data) { data = _data; }
};
