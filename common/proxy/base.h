#pragma once

namespace proxy {
template <class TTValue>
class Base {
 public:
  using TValue = TTValue;
  using TSelf = Base<TValue>;

 protected:
  TValue data;

 public:
  explicit Base(const TValue& _data = TValue()) : data(_data) {}
  const TValue& Get() const { return data; }
  TValue& Get() { return data; }
  void Set(const TValue& _data) { data = _data; }

  bool operator<(const TSelf& r) const { return data < r.data; }
};
}  // namespace proxy
