#pragma once

template <class TValue1, class TFunction>
inline TValue1 FirstIfB(TValue1 begin, TValue1 end, TFunction& f) {
  for (; begin < end; ++begin) {
    if (f(begin)) break;
  }
  return begin;
}

template <class TValue1, class TFunction>
inline TValue1 FirstIfB(TValue1 begin, TFunction& f) {
  for (;; ++begin) {
    if (f(begin)) return begin;
  }
}
