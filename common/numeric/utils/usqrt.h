#pragma once

#include <cmath>
#include <limits>

template<class T>
constexpr T CTUSqrtHelper(T x, T l, T h)
{
    if (l == h) return l;
    T m = (l + h + 1) / 2;
    if (x / m < m)
        return CTUSqrtHelper<T>(x, l, m - 1);
    else
        return CTUSqrtHelper<T>(x, m, h);
}

template<class T>
constexpr T CTUSqrt(T x) { return CTUSqrtHelper<T>(x, 0, x / 2 + 1); }

template<class T>
inline T USqrt(T x)
{
    T r = T(sqrt(double(x)));
    T sqrt_max = CTUSqrt<T>(std::numeric_limits<T>::max());
    for (; (r < sqrt_max) && (r * r < x); ) ++r;
    for (; (r > sqrt_max) || (r * r > x); ) --r;
    return r;
}
