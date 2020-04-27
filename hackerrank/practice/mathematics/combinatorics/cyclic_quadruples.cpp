// https://www.hackerrank.com/challenges/cyclicquadruples

#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_cyclic_quadruples() {
  vector<unsigned> v;

  auto R = [](unsigned l, unsigned r) {
    return TModularD(max(l - 1, r) - (l - 1));
  };

  auto ABCD = [&]() {
    TModularD r = 1;
    for (unsigned i = 0; i < 8; i += 2) r *= R(v[i], v[i + 1]);
    return r;
  };

  auto AACD = [&]() {
    TModularD s = 0;
    for (unsigned j = 0; j < 8; j += 2)
      s += R(max(v[j], v[j + 2]), min(v[j + 1], v[j + 3])) *
           R(v[j + 4], v[j + 5]) * R(v[j + 6], v[j + 7]);
    return s;
  };

  auto AACC = [&]() {
    TModularD s = 0;
    for (unsigned j = 0; j < 4; j += 2)
      s += R(max(v[j], v[j + 2]), min(v[j + 1], v[j + 3])) *
           R(max(v[j + 4], v[j + 6]), min(v[j + 5], v[j + 7]));
    return s;
  };

  auto AAAD = [&]() {
    TModularD s = 0;
    for (unsigned j = 0; j < 8; j += 2)
      s += R(max(max(v[j], v[j + 2]), v[j + 4]),
             min(min(v[j + 1], v[j + 3]), v[j + 5])) *
           R(v[j + 6], v[j + 7]);
    return s;
  };

  auto AAAA = [&]() {
    return R(max(max(v[0], v[2]), max(v[4], v[6])),
             min(min(v[1], v[3]), min(v[5], v[7])));
  };

  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    v = ReadVector<unsigned>(8);
    v.insert(v.end(), v.begin(), v.end());
    cout << ABCD() - AACD() + AACC() + AAAD() - AAAA() * 3 << endl;
  }
  return 0;
}
