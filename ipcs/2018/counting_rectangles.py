import array
import sys

sys.setrecursionlimit(10000)

MOD = 1000000007

_sj = []
for i in range(5001):
    _sj.append(array.array('l', [-1] * (2 * i + 1)))

def surjections(m, n):
    if n < 0 or 2 * m < n:
        return 0
    if m == 0:
        return int(n == 0)
    if _sj[m][n] == -1:
        s = 0
        s += surjections(m - 1, n - 2)
        s += 2 * surjections(m - 1, n - 1)
        s += surjections(m - 1, n)
        s *= n * (n - 1) // 2
        _sj[m][n] = s % MOD
    return _sj[m][n]

def all_surjections(m):
    s = 0
    for im in range(2, 2 * m + 1):
        s += surjections(m, im)
    return s % MOD

_ar = {}

def all_rectangles(m):
    if m not in _ar:
        _ar[m] = (all_surjections(m) ** 2) % MOD
    return _ar[m]

_s = []
for i in range(5010):
    _s.append(array.array('l', [-1] * (i + 1)))

def splits(n, i):
    if i < 0 or i > n:
        return 0
    if n == 0:
        return int(i == 0)
    if _s[n][i] == -1:
        _s[n][i] = (i * splits(n - 1, i) + splits(n - 1, i - 1)) % MOD
    return _s[n][i]

_a = {}

def all_distinct(m):
    if m not in _a:
        s = all_rectangles(m)
        for i in range(0, m):
            s -= (splits(m, i) - MOD) * all_distinct(i)
            s %= MOD
        _a[m] = s
    return _a[m]

_f = [1]

def fact(n):
    while len(_f) <= n:
        _f.append(len(_f) * _f[-1] % MOD)
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
    print (all_distinct(i) * inv(fact(i))) % MOD
