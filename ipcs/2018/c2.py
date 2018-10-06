import array

rects = []

import sys

sys.setrecursionlimit(10000)

MOD = 1000000007

_c = []
for i in range(5100):
    _c.append(array.array('l', [-1] * 11000))


def c(n, k):
    if k < 0 or k > n or n < 0:
        return 0
    if n == 0:
        return int(k == 0)
    if k > n // 2:
        k = n - k
    if k == 0:
        return 1
    if _c[n][k] == -1:
        _c[n][k] = n * c(n - 1, k - 1) * inv(k)
    return _c[n][k] % MOD


_mp = []
for i in range(5100):
    _mp.append(array.array('l', [-1] * 11000))


def mappings(m, n):
    if _mp[m][n] == -1:
        _mp[m][n] = pow(n * (n - 1) // 2, m) % MOD
    return _mp[m][n]


_sj = []
for i in range(5100):
    _sj.append(array.array('l', [-1] * 11000))


def surjections(m, n):
    if n < 0:
        return 0
    if m == 0:
        return int(n == 0)
    if _sj[m][n] == -1:
        xx = (n * (n - 1) // 2) % MOD
        s = 0
        s += surjections(m - 1, n - 2) * xx
        s += surjections(m - 1, n - 1) * xx * 2
        s += surjections(m - 1, n) * xx
        s %= MOD
        _sj[m][n] = s
    return _sj[m][n]



def all_surjections(m):
    s = 0
    for im in range(2, 2 * m + 1):
        s += surjections(m, im)
        if s > MOD:
            s -= MOD
    return s


_ar = {}


def all_rectangles(m):
    if m not in _ar:
        _ar[m] = (all_surjections(m) ** 2) % MOD
    return _ar[m]


_s = []
for i in range(5100):
    _s.append(array.array('l', [-1] * 11000))


def splits(n, i):
    if i < 0 or i > n or n < 0:
        return 0
    if n == 0:
        return int(i == 0)
    if _s[n][i] == -1:
        _s[n][i] = i * splits(n - 1, i) + splits(n - 1, i - 1)
    return _s[n][i] % MOD


_a = {}


def all_distinct(m):
    if m not in _a:
        s = all_rectangles(m)
        for i in range(0, m):
            s -= splits(m, i) * all_distinct(i)
            s += MOD * MOD * MOD
            s %= MOD
        _a[m] = s
    return _a[m]


_f = {}


def fact(n):
    if n == 0:
        return 1
    if n not in _f:
        _f[n] = n * fact(n - 1) % MOD
    return _f[n]


_p = {}


def pow(n, k):
    if k == 0:
        return 1
    if (n, k) not in _p:
        x = pow(n, k // 2) ** 2
        if k % 2 == 1:
            x *= n
        _p[(n, k)] = x % MOD
    return _p[(n, k)]


_i = {}


def inv(n):
    if n not in _i:
        _i[n] = pow(n, MOD - 2) % MOD
    return _i[n]


for i in range(1, 5001):
    # print all_surjections(i)
    print (all_distinct(i) * inv(fact(i))) % MOD
