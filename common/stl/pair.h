#pragma once

#include "../hash.h"
#include <utility>

namespace std {

template <class T1, class T2>
inline pair<T1, T2>& operator+=(pair<T1, T2>& l, const pair<T1, T2>& r) {
  l.first += r.first;
  l.second += r.second;
  return l;
}

template <class T1, class T2>
inline pair<T1, T2>& operator-=(pair<T1, T2>& l, const pair<T1, T2>& r) {
  l.first -= r.first;
  l.second -= r.second;
  return l;
}

template <class T1, class T2>
inline pair<T1, T2> operator+(const pair<T1, T2>& l, const pair<T1, T2>& r) {
  return make_pair(l.first + r.first, l.second + r.second);
}

template <class T1, class T2>
inline pair<T1, T2> operator-(const pair<T1, T2>& l, const pair<T1, T2>& r) {
  return make_pair(l.first - r.first, l.second - r.second);
}

template <class T1, class T2>
struct hash<pair<T1, T2>> {
  size_t operator()(const pair<T1, T2>& value) const {
    return hash_combine(hash<T1>{}(value.first), hash<T2>{}(value.second));
  }
};

}  // namespace std
