#pragma once

#include "common/base.h"
#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

template <class TValue>
class CoordinateCompression {
 protected:
  std::vector<TValue> new_to_old;
  std::unordered_map<TValue, unsigned> old_to_new;

 public:
  CoordinateCompression() {}

  template <class TIterator>
  void InitIterator(TIterator begin, TIterator end, size_t size) {
    new_to_old.clear();
    new_to_old.reserve(size);
    old_to_new.clear();
    for (; begin != end; ++begin) {
      old_to_new[*begin] = unsigned(new_to_old.size());
      new_to_old.push_back(*begin);
    }
  }

  template <class TSortedContainer>
  void InitSorted(const TSortedContainer& v) {
    InitIterator(v.begin(), v.end(), v.size());
  }

  template <class TContainer>
  void InitUnsorted(const TContainer& v) {
    std::set<TValue> s(v.begin(), v.end());
    InitSorted(s);
  }

  template <class TContainer>
  CoordinateCompression(const TContainer& v) {
    InitUnsorted(v);
  }

  unsigned Size() const { return unsigned(new_to_old.size()); }

  const TValue& GetOld(unsigned new_value) const {
    return new_to_old[new_value];
  }

  unsigned GetNew(const TValue& old_value) const {
    auto it = old_to_new.find(old_value);
    assert(it != old_to_new.end());
    return it->second;
  }

  unsigned LowerBound(const TValue& old_value) const {
    auto it = std::lower_bound(new_to_old.begin(), new_to_old.end(), old_value);
    return it - new_to_old.begin();
  }

  unsigned UpperBound(const TValue& old_value) const {
    auto it = std::upper_bound(new_to_old.begin(), new_to_old.end(), old_value);
    return it - new_to_old.begin();
  }
};
