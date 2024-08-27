#pragma once

#include "common/base.h"

#include <vector>

namespace ds {
template <class TValue>
class DequeFixedSize {
 protected:
  std::vector<TValue> data;
  size_t max_size, b, e;

 protected:
  constexpr size_t Next(size_t k) const { return (k + 1) % max_size; }

  constexpr size_t Prev(size_t k) const { return k ? k - 1 : max_size - 1; }

  constexpr size_t Last() const { return Prev(e); }

 public:
  constexpr void Clear() { b = e = 0; }

  constexpr void Reset(size_t new_max_size) {
    max_size = new_max_size + 1;
    data.resize(max_size);
    Clear();
  }

  constexpr explicit DequeFixedSize(size_t _max_size) { Reset(_max_size); }

  constexpr bool Empty() const { return e == b; }

  constexpr size_t Size() const { return (e + max_size - b) % max_size; }

  constexpr bool Full() const { return Next(e) == b; }

  constexpr TValue& Front() { return data[b]; }

  constexpr const TValue& Front() const { return data[b]; }

  constexpr TValue& Back() { return data[Last()]; }

  constexpr const TValue& Back() const { return data[Last()]; }

  constexpr void PopFront() {
    assert(!Empty());
    b = Next(b);
  }

  constexpr void PopBack() {
    assert(!Empty());
    e = Prev(e);
  }

  constexpr void PushFront(const TValue& v) {
    assert(!Full());
    b = Prev(b);
    data[b] = v;
  }

  constexpr void PushBack(const TValue& v) {
    assert(!Full());
    data[e] = v;
    e = Next(e);
  }
};
}  // namespace ds
