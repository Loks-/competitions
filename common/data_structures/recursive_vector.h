#pragma once

#include <vector>

namespace ds {
template <class TValue>
class RecursiveVector {
 public:
  using TSelf = RecursiveVector<TValue>;

 protected:
  bool is_vector;
  TValue value;
  std::vector<TSelf> v;

 public:
  RecursiveVector() : is_vector(true) {}

  RecursiveVector(const TValue& _value) : is_vector(false), value(_value) {}

  RecursiveVector(const std::vector<TValue>& _v) : is_vector(true) {
    v.resize(_v.size());
    for (size_t i = 0; i < Size(); ++i) v[i].SetValue(_v[i]);
  }

  bool IsValue() const { return !is_vector; }
  const TValue& Value() const { return value; }

  bool IsVector() const { return is_vector; }
  const std::vector<TSelf>& Vector() const { return v; }
  size_t Size() const { return v.size(); }

  TSelf& operator[](size_t index) { return v[index]; }
  const TSelf& operator[](size_t index) const { return v[index]; }

  void SetValue(const TValue& _value) {
    is_vector = false;
    value = _value;
    v.clear();
  }
};
}  // namespace ds
