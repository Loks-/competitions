#pragma once

#include <iostream>
#include <utility>

template <class T1, class T2>
inline std::istream& operator>>(std::istream& s, std::pair<T1, T2>& p) {
  s >> p.first >> p.second;
  return s;
}
