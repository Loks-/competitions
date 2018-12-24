// https://www.hackerrank.com/challenges/megaprime-numbers

#include "common/factorization/primality_test.h"
#include "common/stl/base.h"

int main_megaprime_numbers() {
  struct MegaPrime {
    const vector<int> next_digit{2, 2, 3, 5, 5, 7, 7, 2, 2, 2};
    vector<int> v;
    int64_t value;

    void Next() {
      int64_t t = 1;
      for (unsigned i = 0;; ++i) {
        if (i == v.size()) v.push_back(0);
        int cd = v[i], nd = next_digit[v[i]];
        v[i] = nd;
        value += (nd - cd) * t;
        if (nd > cd) break;
        t *= 10;
      }
    }

    MegaPrime(int64_t x) {
      value = x;
      for (; x; x /= 10) v.push_back(x % 10);
      for (unsigned i = unsigned(v.size()); i--;) {
        if ((v[i] != 2) && (v[i] != 3) && (v[i] != 5) && (v[i] != 7)) {
          int64_t t = 1;
          for (unsigned j = 0; j < i; ++j) {
            value += t * (9 - v[j]);
            v[j] = 9;
            t *= 10;
          }
          Next();
          break;
        }
      }
    }
  };

  int64_t f, l;
  cin >> f >> l;
  unsigned total = 0;
  MegaPrime mp(f);
  MillerRabinPrimalityTest pt;
  for (; mp.value <= l; mp.Next()) {
    if (pt.IsPrime(uint64_t(mp.value))) ++total;
  }
  cout << total << endl;
  return 0;
}
