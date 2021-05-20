#pragma once

#include "common/base.h"

#include <vector>

namespace ds {
template <class TValue>
class Deque {
 protected:
  std::vector<TValue> data;
  size_t max_size, b, e;

  size_t Next(size_t k) const { return (k + 1) % max_size; }
  size_t Prev(size_t k) const { return k ? k - 1 : max_size - 1; }
  size_t Last() const { return Prev(e); }

 public:
  void Clear() { b = e = 0; }

  void Reset(size_t new_max_size) {
    max_size = new_max_size + 1;
    data.resize(max_size);
    Clear();
  }

  Deque(size_t _max_size) { Reset(_max_size); }

  bool Empty() const { return e == b; }

  size_t Size() const { return (e + max_size - b) % max_size; }

  bool Full() const { return Next(e) == b; }

  TValue& Front() { return data[b]; }
  const TValue& Front() const { return data[b]; }
  TValue& Back() { return data[Last()]; }
  const TValue& Back() const { return data[Last()]; }

  void PopFront() {
    assert(!Empty());
    b = Next(b);
  }

  void PopBack() {
    assert(!Empty());
    e = Prev(e);
  }

  void PushFront(const TValue& v) {
    assert(!Full());
    b = Prev(b);
    data[b] = v;
  }

  void PushBack(const TValue& v) {
    assert(!Full());
    data[e] = v;
    e = Next(e);
  }
};
}  // namespace ds
