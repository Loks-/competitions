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
  constexpr explicit Base(const TValue& _data = TValue()) : data(_data) {}

  constexpr const TValue& Get() const { return data; }

  constexpr TValue& Get() { return data; }

  constexpr void Set(const TValue& _data) { data = _data; }

  constexpr bool operator<(const TSelf& r) const { return data < r.data; }
};
}  // namespace proxy
