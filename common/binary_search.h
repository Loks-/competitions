#pragma once

template <class TValue1, class TValue2, class TFunction>
inline TValue1 LowerBoundF(TValue1 begin, TValue1 end, const TValue2& value,
                           TFunction& f) {
  for (; begin < end;) {
    TValue1 m = (begin + end) / 2;
    if (f(m) < value)
      begin = m + 1;
    else
      end = m;
  }
  return begin;
}

template <class TValue1, class TValue2, class TFunction>
inline TValue1 UpperBoundF(TValue1 begin, TValue1 end, const TValue2& value,
                           TFunction& f) {
  for (; begin < end;) {
    TValue1 m = (begin + end) / 2;
    if (f(m) <= value)
      begin = m + 1;
    else
      end = m;
  }
  return begin;
}

template <class TValue1, class TFunction>
inline TValue1 LowerBoundB(TValue1 begin, TValue1 end, TFunction& f) {
  for (; begin < end;) {
    TValue1 m = (begin + end) / 2;
    if (f(m))
      end = m;
    else
      begin = m + 1;
  }
  return begin;
}
