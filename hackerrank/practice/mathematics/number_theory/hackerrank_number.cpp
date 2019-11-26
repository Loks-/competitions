// https://www.hackerrank.com/challenges/hackerrank-number

#include "common/factorization/primes_list.h"
#include "common/factorization/utils/coprime_count.h"
#include "common/factorization/utils/coprime_sum.h"
#include "common/factorization/utils/divisors.h"
#include "common/numeric/long/unsigned_io.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_hackerrank_number() {
  uint64_t a, b;
  cin >> a >> b;
  uint64_t raw_b = b;
  if (a > b) swap(a, b);
  factorization::PrimesList primes_list(a);
  // L
  uint64_t cL = 0, sL = 0;
  TFactorization f;
  for (uint64_t p : primes_list.GetPrimes()) {
    uint64_t t = a / p, c = 0;
    for (; t; t /= p) c += 1;
    f.push_back({p, unsigned(c)});
  }
  vector<uint64_t> vd = GetDivisors(f);
  sort(vd.begin(), vd.end());
  uint64_t fd = vd.back();
  for (uint64_t c : vd) {
    uint64_t la = 1;
    for (uint64_t ia = 2; ia <= a; ++ia) {
      if ((c % ia) == 0) la = ia;
    }
    uint64_t cb = b / (c / la), fdc = fd / c;
    TFactorization ff = primes_list.Factorize(fdc);
    cL += CoprimeCount(ff, cb);
    sL += c * CoprimeSum(ff, cb);
  }
  // R
  uint64_t maxaxb = 0;
  for (unsigned i = 30; i--;) {
    uint64_t bit = (1ull << i);
    if ((a | b) & bit) {
      maxaxb |= bit;
      if (a & b & bit) {
        maxaxb += (bit - 1);
        break;
      }
    }
  }
  uint64_t cR = maxaxb + 1, sR = (maxaxb * (maxaxb + 1)) / 2;
  if (a == 1) {
    cR -= 1;
    sR -= a;  // a is not possible
    if ((b & 1) == 0) {
      cR -= 1;
      sR -= b;  // b is not possible
    }
  } else {
    if ((b ^ (b - 1)) / 2 >= a) {
      cR -= 1;
      sR -= b;  // b is not possible
    }
  }
  // Final
  uint64_t lb = 1, pb = 10;
  for (; raw_b >= pb; pb *= 10) ++lb;
  LongUnsigned r1 = LongUnsigned(sL) * cR * pb * 10u;
  LongUnsigned r2 = LongUnsigned(sR) * cL;
  cout << r1 + r2 << endl;
  return 0;
}
